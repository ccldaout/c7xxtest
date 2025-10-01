#include <c7app.hpp>
#include <c7thread/datapara_flex.hpp>

using c7::p_;

namespace dp = c7::thread::datapara;

struct in_data {
    int value;
};

int main()
{
    dp::configure cfg;
    cfg.mt_threshold = 10;
    cfg.n_thread = 3;
    cfg.n_item_per_thread = 7;

    auto cal = dp::make_driver<in_data, std::string>(
	"my_calcu",
	[](int th_idx, in_data& v, std::string& o) {
	    o = c7::format("0x%{04x}", v.value);
	},
	[](std::string& o) {
	    p_("post_process: <%{}>", o);
	},
	[](const char *src_name, int src_line,
	   uint32_t level, const std::string& s) {
	    p_("%{}:%{} #%{} %{}", src_name, src_line, level, s);
	});

    cal->init(cfg);
    cal->start_round();

    in_data id;
    for (int i = 0; i < 30; i++) {
	id.value = i;
	if (auto res = cal->put(id); !res) {
	    p_("put: %{}", res);
	    break;
	}
    }

    if (auto res = cal->end_round(); !res) {
	p_("end_round: %{}", res);
    }
    cal->end();
}
