#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7mlog.hpp>

c7::mlog_writer mlog;

#define mlog(_lv, ...)	\
    mlog.format(__FILE__, __LINE__, C7_LOG_##_lv, 0, 0, __VA_ARGS__)

#include <c7nseq/generator.hpp>
#include <c7nseq/list.hpp>
#include <c7nseq/range.hpp>
#include <c7nseq/reverse.hpp>
#include <c7nseq/tail.hpp>
#include <c7nseq/vector.hpp>
#include <c7string/strvec.hpp>


using c7::p_;
using c7::P_;
using namespace c7::nseq;


[[maybe_unused]] static void break_point()
{
}


static auto s_range(size_t n)
{
    return c7::nseq::generator<size_t>(
	[n](auto& out) {
	    for (size_t i = 0; i < n; i++) {
		out << i;
	    }
	})();
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

    p_("\n[drop_tail] --------------------------------------------------------------");
    {
	c7::strvec kws{"zero", "one", "two", "three", "four", "five", "six", "seven"};
	p_("  kws %{}", kws);
	p_("  kws | drop_tail(3)");
	p_("   -> %{}",
	   kws | drop_tail(3));
    }
    {
	p_("  s_range(0) | drop_tail(3) %{} -> %{}",
	   s_range(0), s_range(0) | drop_tail(3));
	p_("  s_range(1) | drop_tail(3) %{} -> %{}",
	   s_range(1), s_range(1) | drop_tail(3));
	p_("  s_range(2) | drop_tail(3) %{} -> %{}",
	   s_range(2), s_range(2) | drop_tail(3));
	p_("  s_range(3) | drop_tail(3) %{} -> %{}",
	   s_range(3), s_range(3) | drop_tail(3));
	p_("  s_range(4) | drop_tail(3) %{} -> %{}",
	   s_range(4), s_range(4) | drop_tail(3));
	p_("  s_range(5) | drop_tail(3) %{} -> %{}",
	   s_range(5), s_range(5) | drop_tail(3));
	p_("  s_range(6) | drop_tail(3) %{} -> %{}",
	   s_range(6), s_range(6) | drop_tail(3));
	p_("  s_range(7) | drop_tail(3) %{} -> %{}",
	   s_range(7), s_range(7) | drop_tail(3));
	p_("  s_range(8) | drop_tail(3) %{} -> %{}",
	   s_range(8), s_range(8) | drop_tail(3));
	p_("  s_range(9) | drop_tail(3) %{} -> %{}",
	   s_range(9), s_range(9) | drop_tail(3));
	p_("  s_range(10) | drop_tail(3) %{} -> %{}",
	   s_range(10), s_range(10) | drop_tail(3));
    }
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_tail();
}
