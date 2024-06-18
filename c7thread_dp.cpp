#include <c7app.hpp>
#include <c7thread/datapara.hpp>

using c7::p_;

namespace dp = c7::thread::datapara;

struct in_data {
    int value;
    char pad[1024*1024/5];
};

class my_calcu: public dp::driver<my_calcu, in_data, std::string> {
public:
    const char *datapara_name() {
	return "my_calcu";
    }

    void datapara_main_process(int th_idx, in_data& v, std::string& o) {
	o = c7::format("0x%{04x}", v.value);
    }

    void datapara_post_process(std::string& o) {
	p_("post_process: <%{}>", o);
    }

    void datapara_logger(const char *src_name, int src_line,
			 uint32_t level, const std::string& s) {
	p_("%{}:%{} #%{} %{}", src_name, src_line, level, s);
    }
};

int main()
{
    my_calcu cal;

    dp::configure cfg;
    cfg.n_thread = 3;
    cfg.mt_threshold = 10;
    cfg.bufsize_mb = 1;

    cal.init(cfg);
    cal.start_round();

    in_data id;
    for (int i = 0; i < 30; i++) {
	id.value = i;
	if (auto res = cal.put(id); !res) {
	    p_("put: %{}", res);
	    break;
	}
    }

    if (auto res = cal.end_round(); !res) {
	p_("end_round: %{}", res);
    }
    cal.end();
}
