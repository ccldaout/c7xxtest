#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/reverse.hpp>
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


[[maybe_unused]] static void test_sort()
{
    c7::strvec sv{"one", "two", "three", "four"};

    p_("\n[sort] --------------------------------------------------------------");
    p_("  sv %{}", sv);

    p_("  sv | sort");
    p_("     %{}", sv | sort());
    p_("  sv %{}", sv);

    p_("  ... | reverse | sort_by(>)");
    p_("     %{}", sv | reverse{} | sort_by(std::greater<std::string>{}));
    p_("  sv %{}", sv);
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_sort();
}
