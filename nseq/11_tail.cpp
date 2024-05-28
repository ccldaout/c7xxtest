#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/list.hpp>
#include <c7nseq/range.hpp>
#include <c7nseq/reverse.hpp>
#include <c7nseq/tail.hpp>
#include <c7string.hpp>
#include <c7mlog.hpp>


c7::mlog_writer mlog;

#define mlog(_lv, ...)	\
    mlog.format(__FILE__, __LINE__, C7_LOG_##_lv, 0, 0, __VA_ARGS__)

using c7::p_;
using c7::P_;
using namespace c7::nseq;


[[maybe_unused]] static void break_point()
{
}


[[maybe_unused]] static void test_tail()
{
    p_("\n[tail] --------------------------------------------------------------");
    {
	c7::strvec kws{"zero", "one", "two", "three", "four", "five", "six", "seven"};
	p_("  kws %{}", kws);
	p_("  kws | tail(3)");
	p_("   -> %{}",
	   kws | tail(3));
	p_("  kws | tail(3) | reverse(");
	p_("   -> %{}",
	   kws | tail(3) | reverse());
	p_("  kws (after)");
	p_("   -> %{}",
	   kws);
    }

    {
	p_("  range(100) | tail(3)");
	p_("   -> %{}",
	   range(100) | tail(3));
	p_("  range(100) | tail(3) | reverse");
	p_("   -> %{}",
	   range(100) | tail(3) | reverse());
    }

    {
	p_("  range(100) | to_list | tail(3)");
	p_("   -> %{}",
	   range(100) | to_list() | tail(3));
#if 0	// COMPILE ERROR [OK]
	p_("  range(100) | to_list | tail(3) | reverse");
	p_("   -> %{}",
	   range(8) | to_list() | tail(3) | reverse());
#endif
    }
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_tail();
}
