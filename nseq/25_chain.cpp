#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7path.hpp>
#include <c7mlog.hpp>
#include <c7nseq/chain.hpp>
#include <c7nseq/head.hpp>
#include <c7nseq/range.hpp>
#include <c7nseq/reverse.hpp>
#include <c7nseq/tail.hpp>


using namespace c7::nseq;


c7::mlog_writer mlog;

#define mlog(_lv, ...)	\
    mlog.format(__FILE__, __LINE__, C7_LOG_##_lv, 0, 0, __VA_ARGS__)

using c7::p_;
using c7::P_;

[[maybe_unused]] static void break_point()
{
}


[[maybe_unused]] static void test_chain()
{
    p_("\n[test_chain] -----------------------------------------------------------");

    p_(" chain(range, range|tail|reverse");
    p_("  -> %{}",
       chain(range(5),
	     range(99)|tail(5)|reverse())
	);
    p_(" range, (range|tail|reverse)");
    p_("  -> %{}",
       (range(5),
	range(99) | tail(5) | reverse())
	);

    p_(" (range|head), (range|tail|reverse)");
    p_("  -> %{}",
       (range(10) | head(5),
	range(99) | tail(5) |reverse())
	);

    p_(" range, range, range");
    p_("  -> %{}",
       (range(5),
	range(5, 10),
	range(5, 100))
	);
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_chain();
}
