#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <c7app.hpp>
#include <c7path.hpp>
#include <c7progmon.hpp>


/*----------------------------------------------------------------------------
                  c7::progress_monitor を使ったサンプル実装
----------------------------------------------------------------------------*/

struct prog_data_t {
    uint64_t total;
    uint64_t count;
};


class MyProgress: public c7::progress_monitor<prog_data_t> {
    using base_type = c7::progress_monitor<prog_data_t>;
public:
    void start(size_t total, c7::usec_t interval_ms);
    void count_up();
    void stop();

private:
    // 親側で設定
    std::string item_name_;

    // 子プロセス側
    int tty_fd_;

    // 次の二つは c7::progress_monitor<> からの要件
    void monitoring_init() override;
    void monitoring_check(uint64_t not_updated_count,
			  pid_t parent_pid,
			  prog_data_t& data) override;

    void print(prog_data_t& data, bool last);
};


// [親プロセスで呼ぶ]
void
MyProgress::start(size_t total, c7::usec_t interval_ms)
{
    // 進捗データの初期化
    auto& data = base_type::update();
    data.total = total;
    data.count = 0;

    // 進捗率の右側に表示するアイテム名
    // 現在ディレクトリを取得していることに特に意味はない (ただのサンプルプログラム)。
    item_name_ = c7::path::cwd().value(std::string{"Oops! c7::path::cwd() failed"});

    // バックグラウンドモニタリングループを開始する
    base_type::start_monitor(interval_ms);
}


// [親プロセスで呼ぶ]
void
MyProgress::count_up()
{
    base_type::update().count++;
}


// [親プロセスで呼ぶ]
void
MyProgress::stop()
{
    base_type::stop_monitor();
}


static size_t term_cols;

static size_t get_cols(int fd)
{
    struct winsize wz;
    if (ioctl(fd, TIOCGWINSZ, &wz) == 0) {
	return wz.ws_col;
    }
    return 80;
}

static void sig_winch(int)
{
    // 端末ウィンドウのサイズが変化した!!
    
    // シグナルハンドラ内で呼び出せる関数・システムコールは限られているので、
    // このハンドラでは静的変数をリセットするに留める。
    term_cols = 0;
}


// [子プロセスで最初に呼ばれる]
void
MyProgress::monitoring_init()
{
    // 基底クラスの monitoring_init は 3..256 までの記述子をクローズする。
    base_type::monitoring_init();

    // 標準出力がリダイレクトされている可能性もあるので端末かチェックし、
    // 端末でなければ /dev/tty を直接開ける。
    if (isatty(1)) {
	tty_fd_ = 1;
    } else {
	tty_fd_ = ::open("/dev/tty", O_WRONLY);
    }

    // 端末エミュレータのウィンドウサイズが変更された時のシグナルハンドラ
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sig_winch;
    sigaction(SIGWINCH, &sa, nullptr);
}


// [子プロセスで定期的に呼ばれる]
void
MyProgress::monitoring_check(uint64_t, pid_t, prog_data_t& data)
{
    print(data, false);
}


// 進捗表示のサンプル実装
void
MyProgress::print(prog_data_t& data, bool last)
{
    if (term_cols == 0) {
	// 一発目の呼び出しか、端末ウィンドウのサイズが変更された。
	// いずれにしろ、tty から端末のコラム数を取得する。
	// +4 は、出力文字列の先頭4バイトに位置する ANSIエスケープ
	// シーケンスの分。サイズの比較時のために、その分を term_cols
	// に上乗せしておく。
	term_cols = get_cols(tty_fd_) + 4;
    }

    double rate;
    if (data.total == 0) {
	rate = 0.0;
    } else {
	rate = (100.0 * data.count) / data.total;
    }

    const char *pref = "\x1b[2K";	// 行全体をクリア
    const char *sffx = "\r";		// カーソルを先頭に戻す
    if (last) {
	// 最後の出力では ...
	sffx = "\n";		// カーソルを次の行の先頭に
	term_cols = 1024;	// 折り返しても良いので全体を表示する (ために、わざと大きな幅を設定)
    }

    std::string s =
	c7::format("%{}  %{8.3f}%%  [%{}/%{}] %{}%{}",
		   pref, rate, data.count, data.total, item_name_, sffx);
    if (s.size() >= term_cols) {
	s.resize(term_cols-2);
	s.push_back('\r');
    }

    // c7::drop はなんでも代入できる(が、捨てさる)。
    // コンパイラの「write() の戻り値を捨てるな」警告(エラー)対策
    c7::drop = ::write(tty_fd_, s.c_str(), s.size());
}


/*----------------------------------------------------------------------------
                                 テストコード
----------------------------------------------------------------------------*/

#include <c7nseq/random.hpp>

using c7::p_;
using c7::P_;

int main()
{
    size_t total = 10000UL;
    constexpr c7::usec_t itv_ms = 200;

    MyProgress prg;
    prg.start(total, itv_ms);

    for (auto us: c7::nseq::random_uniform_dist(0, 100UL, 500UL)) {
	c7::sleep_us(1000UL + us);
	prg.count_up();
	if (--total == 0) {
	    break;
	}
    }

    prg.stop();
}
