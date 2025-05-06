#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/group.hpp>
#include <c7nseq/list.hpp>
#include <c7nseq/sort.hpp>
#include <c7string/strvec.hpp>
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


[[maybe_unused]] static void test_group()
{
    c7::strvec data{"zero", "one", "two", "three", "four", "five", "six", "seven"};

    p_("\n[group] -------------------------------------------------------------");
    p_("  kws   %{}", data);
    p_("  group_*_by a[0] == b[0]");
    {
	auto kws = data;
	p_("  kws | sort | group_by");
	p_("   -> %{}",
	   kws | sort() | group_by([](auto& a, auto& b){ return a[0] == b[0]; }));
    }
    {
	auto kws = data;
	p_("   kws | sort | to_list | group_by");
	p_("   -> %{}",
	   kws | sort() | to_list() | group_by([](auto& a, auto& b){ return a[0] == b[0]; }));
    }
    {
	auto kws = data;
	p_("   kws | sort | group_range_by");
	p_("   -> %{}",
	   kws | sort() | group_range_by([](auto& a, auto& b){ return a[0] == b[0]; }));
    }
    {
	auto kws = data;
	p_("  kws | sort | to_list | group_range_by");
	p_("   -> %{}",
	   kws | sort() | to_list() | group_range_by([](auto& a, auto& b){ return a[0] == b[0]; }));
    }

    p_("  group_*_by ++n % 3 != 0");
    {
	auto kws = data;
	p_("  kws | group_range_by");
	p_("   -> %{}",
	   kws | group_range_by([n=0](auto,auto) mutable { return ++n % 3 != 0; }));
    }
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_group();
}
