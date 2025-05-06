#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/enumerate.hpp>
#include <c7nseq/reverse.hpp>
#include <c7nseq/sort.hpp>
#include <c7nseq/vector.hpp>
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


[[maybe_unused]] static void test_to_vector()
{
    p_("\n[to_vector] ---------------------------------------------------------");

    std::list<std::string> ls{"one", "two", "three", "four", "five", "six"};
    p_("  list");
    p_("   -> %{}", ls);
    p_("  ls | reverse | enumerate | to_vector");
    auto seq = ls | reverse{} | enumerate() | to_vector();
    auto it = seq.begin();
    p_("   -> %{}", seq);
    p_("  it[4] -> %{}", it[4]);
    p_("  it[2] -> %{}", it[2]);
    p_("  it[5] -> %{}", it[5]);
    p_("      list");
    p_("   -> %{}", ls);
    seq | sort();
    p_("  seq|sort");
    p_("   -> %{}", seq);
    p_("      list");
    p_("   -> %{}", ls);
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_to_vector();
}
