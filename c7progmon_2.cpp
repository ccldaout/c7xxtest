#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <c7app.hpp>
#include <c7path.hpp>
#include <c7progmon.hpp>


/*----------------------------------------------------------------------------
                  c7::progress_monitor ��Ȥä�����ץ����
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
    // ��¦������
    std::string item_name_;

    // �ҥץ���¦
    int tty_fd_;

    // ������Ĥ� c7::progress_monitor<> ������׷�
    void monitoring_init() override;
    void monitoring_check(uint64_t not_updated_count,
			  pid_t parent_pid,
			  prog_data_t& data) override;

    void print(prog_data_t& data, bool last);
};


// [�ƥץ����ǸƤ�]
void
MyProgress::start(size_t total, c7::usec_t interval_ms)
{
    // ��Ľ�ǡ����ν����
    auto& data = base_type::update();
    data.total = total;
    data.count = 0;

    // ��ĽΨ�α�¦��ɽ�����륢���ƥ�̾
    // ���ߥǥ��쥯�ȥ��������Ƥ��뤳�Ȥ��ä˰�̣�Ϥʤ� (�����Υ���ץ�ץ����)��
    item_name_ = c7::path::cwd().value(std::string{"Oops! c7::path::cwd() failed"});

    // �Хå����饦��ɥ�˥���󥰥롼�פ򳫻Ϥ���
    base_type::start_monitor(interval_ms);
}


// [�ƥץ����ǸƤ�]
void
MyProgress::count_up()
{
    base_type::update().count++;
}


// [�ƥץ����ǸƤ�]
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
    // ü��������ɥ��Υ��������Ѳ�����!!
    
    // �����ʥ�ϥ�ɥ���ǸƤӽФ���ؿ��������ƥॳ����ϸ¤��Ƥ���Τǡ�
    // ���Υϥ�ɥ�Ǥ���Ū�ѿ���ꥻ�åȤ����α��롣
    term_cols = 0;
}


// [�ҥץ����Ǻǽ�˸ƤФ��]
void
MyProgress::monitoring_init()
{
    // ���쥯�饹�� monitoring_init �� 3..256 �ޤǤε��һҤ򥯥������롣
    base_type::monitoring_init();

    // ɸ����Ϥ�������쥯�Ȥ���Ƥ����ǽ���⤢��Τ�ü���������å�����
    // ü���Ǥʤ���� /dev/tty ��ľ�ܳ����롣
    if (isatty(1)) {
	tty_fd_ = 1;
    } else {
	tty_fd_ = ::open("/dev/tty", O_WRONLY);
    }

    // ü�����ߥ�졼���Υ�����ɥ����������ѹ����줿���Υ����ʥ�ϥ�ɥ�
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sig_winch;
    sigaction(SIGWINCH, &sa, nullptr);
}


// [�ҥץ��������Ū�˸ƤФ��]
void
MyProgress::monitoring_check(uint64_t, pid_t, prog_data_t& data)
{
    print(data, false);
}


// ��Ľɽ���Υ���ץ����
void
MyProgress::print(prog_data_t& data, bool last)
{
    if (term_cols == 0) {
	// ��ȯ�ܤθƤӽФ�����ü��������ɥ��Υ��������ѹ����줿��
	// ������ˤ���tty ����ü���Υ�������������롣
	// +4 �ϡ�����ʸ�������Ƭ4�Х��Ȥ˰��֤��� ANSI����������
	// �������󥹤�ʬ������������ӻ��Τ���ˡ�����ʬ�� term_cols
	// �˾�褻���Ƥ�����
	term_cols = get_cols(tty_fd_) + 4;
    }

    double rate;
    if (data.total == 0) {
	rate = 0.0;
    } else {
	rate = (100.0 * data.count) / data.total;
    }

    const char *pref = "\x1b[2K";	// �����Τ򥯥ꥢ
    const char *sffx = "\r";		// �����������Ƭ���᤹
    if (last) {
	// �Ǹ�ν��ϤǤ� ...
	sffx = "\n";		// ��������򼡤ιԤ���Ƭ��
	term_cols = 1024;	// �ޤ��֤��Ƥ��ɤ��Τ����Τ�ɽ������ (����ˡ��虜���礭����������)
    }

    std::string s =
	c7::format("%{}  %{8.3f}%%  [%{}/%{}] %{}%{}",
		   pref, rate, data.count, data.total, item_name_, sffx);
    if (s.size() >= term_cols) {
	s.resize(term_cols-2);
	s.push_back('\r');
    }

    // c7::drop �Ϥʤ�Ǥ������Ǥ���(�����ΤƤ���)��
    // ����ѥ���Ρ�write() ������ͤ�ΤƤ�ʡ׷ٹ�(���顼)�к�
    c7::drop = ::write(tty_fd_, s.c_str(), s.size());
}


/*----------------------------------------------------------------------------
                                 �ƥ��ȥ�����
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
