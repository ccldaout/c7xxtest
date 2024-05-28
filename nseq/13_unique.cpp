#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/head.hpp>
#include <c7nseq/list.hpp>
#include <c7nseq/sort.hpp>
#include <c7nseq/tail.hpp>
#include <c7nseq/unique.hpp>
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


[[maybe_unused]] static void test_unique()
{
    c7::strvec data{"C", "JAVA", "Go", "C#", "Rust", "C++", "JAVA", "Go", "Rust", "Go"};

    p_("\n[unique] ------------------------------------------------------------");
    p_("  kws   %{}", data);
    {
	auto kws = data;
	p_("  kws | sort | unique");
	p_("   -> %{}",
	   kws | sort() | unique());
	kws = data;
	p_("  kws | sort | unique | head(2)");
	p_("   -> %{}",
	   kws | sort() | unique() | head(2));
	kws = data;
	p_("  kws | sort | unique | tail(2)");
	p_("   -> %{}",
	   kws | sort() | unique() | tail(2));
    }

    {
	auto kws = data;
	auto ls = kws | sort() | to_list();
	p_("  kwd | sort | to_list");
	p_("   -> %{}", ls);
	p_("  kws | sort | to_list | unique");
	p_("   -> %{}",
	   ls | unique());
	p_("  kws | sort | to_list | unique | head(2)");
	p_("   -> %{}",
	   ls | unique() | head(2));
	p_("  kws | sort | to_list | unique | tail(2)");
	p_("   -> %{}",
	   ls | unique() | tail(2));
    }
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_unique();
}
