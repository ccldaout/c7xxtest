#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/c_array.hpp>
#include <c7nseq/head.hpp>
#include <c7nseq/sort.hpp>
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


[[maybe_unused]] static void test_c_array()
{
    p_("\n[c_array] -----------------------------------------------------------");
    {
	std::string kws[] = { "zero", "one", "two", "three", "four", "five", "six", "seven" };
	p_("- std::string[]");
	p_("  c_array(kws)");
	p_("   -> %{}",
	   c_array(kws));
	p_("  c_array(kws | sort | head(4)");
	p_("   -> %{}",
	   c_array(kws) | sort() | head(4));
	p_(" c_array(kws) (redisplay)");
	p_("   -> %{}",
	   c_array(kws));
    }

    auto c_str_cmp = [](auto& p1, auto& p2) { return std::strcmp(p1, p2) < 0; };

    {
	const char *kws[] = { "zero", "one", "two", "three", "four", "five", "six", "seven" };
	p_("- const char*[]");
	p_("  c_array(kws)");
	p_("   -> %{}",
	   c_array(kws));
	p_("  c_array(kws) | sort_by | head(4)");
	p_("   -> %{}",
	   c_array(kws) | sort_by(c_str_cmp) | head(4));
	p_("  c_array(kws) (redisplay)");
	p_("   -> %{}",
	   c_array(kws));
    }

    {
	const char *kws[] = { "zero", "one", "two", "three", "four", "five", "six", "seven" };
	const char **kwsp = kws;
	p_("- const char**");
	p_("  c_array(kwsp, 8)");
	p_("   -> %{}",
	   c_array(kwsp, 8));
	p_("  c_array(kwsp, 8) | sort_by | head(4)");
	p_("   -> %{}",
	   c_array(kwsp, 8) | sort_by(c_str_cmp) | head(4));
	p_("  c_array(kws) (redisplay)");
	p_("   -> %{}",
	   c_array(kws));
    }
    {
	const char *kws[] = { "zero", "one", "two", "three", "four", "five", "six", "seven", nullptr };
	const char **kwsp = kws;
	p_("- const char** (nullptr terminated)");
	p_("  c_array(kwsp, nullptr)");
	p_("   -> %{}",
	   c_array(kwsp, nullptr));
    }
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_c_array();
}
