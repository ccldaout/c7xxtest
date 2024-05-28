#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7path.hpp>
#include <c7string.hpp>
#include <c7mlog.hpp>

#include <c7nseq/enumerate.hpp>
#include <c7nseq/range.hpp>
#include <c7nseq/window.hpp>


using namespace c7::nseq;


c7::mlog_writer mlog;

#define mlog(_lv, ...)	\
    mlog.format(__FILE__, __LINE__, C7_LOG_##_lv, 0, 0, __VA_ARGS__)

using c7::p_;
using c7::P_;

[[maybe_unused]] static void break_point()
{
}


[[maybe_unused]] static void test_window()
{
    p_("\n[test_window] -----------------------------------------------------------");

    p_(" range | window_list");
    p_("  -> %{}",
       range(7) | window_list(3)
	);
    p_(" range | window_range");
    p_("  -> %{}",
       range(7) | window_range(3)
	);

    c7::strvec kv{"zero", "one", "two", "three", "four"};
    p_(" kv | window_list");
    p_("  -> %{}",
       kv | window_list(3)
	);
    p_(" kv | window_range");
    p_("  -> %{}",
       kv | window_range(3)
	);

    p_(" kv | enumerate | window_list");
    p_("  -> %{}",
       kv | enumerate() | window_list(3)
	);
    p_(" kv | enumerate | window_range");
    p_("  -> %{}",
       kv | enumerate() | window_range(3)
	);
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_window();
}
