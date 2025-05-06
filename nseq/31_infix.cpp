#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7path.hpp>
#include <c7mlog.hpp>
#include <c7nseq/infix.hpp>
#include <c7nseq/range.hpp>


using namespace c7::nseq;


c7::mlog_writer mlog;

#define mlog(_lv, ...)	\
    mlog.format(__FILE__, __LINE__, C7_LOG_##_lv, 0, 0, __VA_ARGS__)

using c7::p_;
using c7::P_;

[[maybe_unused]] static void break_point()
{
}


[[maybe_unused]] static void test_infix()
{
    p_("\n[test_infix] -----------------------------------------------------------");

    p_("range(0) -> %{}",
       c7::nseq::range(0)
       | c7::nseq::infix([](auto&& a, auto&& b){ return (a+b)*100; })
	);

    p_("range(1) -> %{}",
       c7::nseq::range(1)
       | c7::nseq::infix([](auto&& a, auto&& b){ return (a+b)*100; })
	);

    p_("range(2) -> %{}",
       c7::nseq::range(2)
       | c7::nseq::infix([](auto&& a, auto&& b){ return (a+b)*100; })
	);

    p_("range(9) -> %{}",
       c7::nseq::range(9)
       | c7::nseq::infix([](auto&& a, auto&& b){ return (a+b)*100; })
	);
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_infix();
}
