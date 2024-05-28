#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/while.hpp>
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


[[maybe_unused]] static void test_while()
{
    c7::strvec kws{ "zero", "one", "two", "three", "four", "five", "six", "seven" };

    p_("\n[while] -------------------------------------------------------------");
    p_("  kws   %{}", kws);
    {
	p_("  kws | take_while(len < 5)");
	p_("   -> %{}",
	   kws | take_while([](auto& s){ return s.size() < 5; }));
    }
    {
	p_("  kws | skip_while(len < 5)");
	p_("   -> %{}",
	   kws | skip_while([](auto& s){ return s.size() < 5; }));
    }
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_while();
}
