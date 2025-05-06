#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/accumulate.hpp>
#include <c7nseq/list.hpp>
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


[[maybe_unused]] static void test_accumulate()
{
    c7::strvec kws{"zero", "one", "two", "three", "four", "five", "six", "seven"};

    p_("\n[accumulate] --------------------------------------------------------");
    p_("  kws | accumulate(a+,+b)");
    p_("   -> %{}",
       kws | accumulate([](auto& a, auto& b){ return a + "," + b; })
	);
    p_("  kws | accumulate(std::plus, ...)");
    p_("   -> %{}",
       kws | accumulate(std::plus{}, std::string("... "))
	);
    p_("  kws | to_list | accumulate(a+,+b)");
    p_("   -> %{}",
       kws | to_list() | accumulate([](auto& a, auto& b){ return a + "," + b; })
	);
    p_("  kws | to_list | accumulate(std::plus, ...");
    p_("   -> %{}",
       kws | to_list() | accumulate(std::plus{}, std::string("... "))
	);
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_accumulate();
}
