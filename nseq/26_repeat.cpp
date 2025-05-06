#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7path.hpp>
#include <c7string/strvec.hpp>
#include <c7mlog.hpp>
#include <c7nseq/chain.hpp>
#include <c7nseq/command.hpp>
#include <c7nseq/head.hpp>
#include <c7nseq/range.hpp>
#include <c7nseq/repeat.hpp>
#include <c7nseq/zip.hpp>


using namespace c7::nseq;


c7::mlog_writer mlog;

#define mlog(_lv, ...)	\
    mlog.format(__FILE__, __LINE__, C7_LOG_##_lv, 0, 0, __VA_ARGS__)

using c7::p_;
using c7::P_;

[[maybe_unused]] static void break_point()
{
}


[[maybe_unused]] static void test_repeat()
{
    p_("\n[test_repeat] -----------------------------------------------------------");

    p_(" range|repeat");
    p_("  -> %{}",
       range(5) | repeat(3)
	);

    c7::strvec kv{"c++17", "c++20", "c99"};
    p_(" zip(range(10), {'c++17', 'c++20', 'c99'}|repeat(3))");
    p_("  -> %{}",
       zip(range(10), kv | repeat())
	);

    c7::strvec av{"cat", "main.cpp"};
    p_(" command | head | repeat(3)");
    for (auto s: command<char>(av)() | head(5) | repeat(3)) {
	p_("  '%{}'", s);
    }

    p_(" (kv, {\"--\"} | repeat) | head");
    p_("  -> %{}",
       (kv, c7::strvec{"--"} | repeat()) | head(8)
	);
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_repeat();
}
