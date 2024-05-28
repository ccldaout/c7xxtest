#include <c7app.hpp>
#include <c7thread/msgbox.hpp>
#include <c7thread/thread.hpp>
#include <c7nseq/range.hpp>

namespace ns = c7::nseq;

using c7::p_;
using c7::P_;

struct data_t {
    std::string s;
    int v;
};


template
class c7::thread::msgbox<data_t>;


c7::thread::msgbox<data_t> mbox;

using u64_key = c7::thread::msgbox<data_t>::u64_key;
using tray_key = c7::thread::msgbox<data_t>::tray_key;

static void proc(tray_key key)
{
    auto self_id = c7::thread::self::id();
    c7::usec_t tmo = 3*1000*1000;

    for (;;) {
	p_("#%{}: mbox.wait(key:%{}) ...", self_id, key);
	if (auto res = mbox.wait(key, tmo); !res) {
	    p_("#%{}: mbox.wait(key:%{}) failed\n%{}", self_id, key, res);
	    return;
	} else {
	    auto msgs = std::move(res.value());
	    p_("#%{}: mbox.wait(key:%{}) -> #msg:%{}", self_id, key, msgs.size());
	    for (auto& m: msgs) {
		p_("#%{}: m.v: %{}", self_id, m.v);
	    }
	}
    }
}

static constexpr int N = 3;

int main()
{
    {
	struct th_attr {
	    c7::thread::thread th;
	    tray_key key;
	};

	std::vector<th_attr> ths;
	for (int i = 0; i < N; i++) {
	    th_attr ta;
	    ta.key = mbox.reserve();
	    ta.th.target([key=ta.key]() { proc(key); });
	    if (auto res = ta.th.start(); !res) {
		c7error("thread start failed: %{}", res);
	    }
	    ths.push_back(std::move(ta));
	}

	p_("#mbox: %{}", mbox.size());

	for (auto v: ns::range(50)) {
	    auto i = v % N;
	    data_t data{};
	    data.v = v;
	    mbox.put(ths[i].key(), data);
	    if ((v % 17) == 1) {
		c7::sleep_us(10);
	    }
	}

	for (auto& th: ths) {
	    th.th.join();
	}
    }
    p_("#mbox: %{}", mbox.size());
}
