#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/enumerate.hpp>
#include <c7nseq/filter.hpp>
#include <c7nseq/reverse.hpp>
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


[[maybe_unused]] static void test_random_access()
{
    p_("\n[reversed & random access] ------------------------------------------");

    c7::strvec sv{"one", "two", "three", "four", "five", "six"};
    p_("  sv");
    p_("   -> %{}", sv);
    p_("  sv | reverse | enumerate");
    auto seq = sv | reverse{} | enumerate();
    auto it = seq.begin();
    p_("   -> %{}", seq);
    p_("  it[4] -> %{}", it[4]);
    p_("  it[2] -> %{}", it[2]);
    p_("  it[5] -> %{}", it[5]);
    p_("  sv");
    p_("   -> %{}", sv);
    p_("  - rewrite it[1].second ... (it is reversed iterator)");
    it[1].second = "rewrite";
    p_("  sv");
    p_("   -> %{}", sv);
}


#if 0
// COMPILE ERROR [OK]
[[maybe_unused]] static void test_random_access_list()
{
    std::list<std::string> ls{"one", "two", "three", "four", "five", "six"};
    auto seq = ls | reverse{} | enumerate();
    auto it = seq.begin();
    p_("   it[4]");
    p_("   -> %{}", it[4]);
    p_("   it[2]");
    p_("   -> %{}", it[2]);
    p_("   it[5]");
    p_("   -> %{}", it[5]);
}
#endif


[[maybe_unused]] static void test_reverse_for_list()
{
    std::list<std::string> ls{"one", "two", "three", "four"};

    p_("\n[reverse for list] --------------------------------------------------");
    p_("  ls %{}", ls);

    p_("  ls | reverse");
    p_("   -> %{}", ls | reverse{});
    p_("  ls | reverse | enumerate");
    p_("   -> %{}", ls | reverse{} | enumerate());
    p_("  ls | enumerate | reverse");
    p_("   -> %{}", ls | enumerate() | reverse{});
}


[[maybe_unused]] static void test_reverse()
{
    c7::strvec sv{"one", "two", "three", "four"};

    p_("\n[reverse for vector] ------------------------------------------------");
    p_("  sv %{}", sv);

    p_("  sv | reverse");
    p_("   -> %{}", sv | reverse{});
    p_("  sv | reverse | enumerate");
    p_("   -> %{}", sv | reverse{} | enumerate());
    p_("  sv | enumerate | reverse");
    p_("   -> %{}", sv | enumerate() | reverse{});
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_reverse();
    test_reverse_for_list();
    test_random_access();
}
