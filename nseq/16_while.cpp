#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/chain.hpp>
#include <c7nseq/range.hpp>
#include <c7nseq/while.hpp>
#include <c7nseq/vector.hpp>
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


[[maybe_unused]] static void test_while()
{
    c7::strvec kws{ "zero", "one", "two", "three", "four", "five", "six", "seven" };

    p_("\n[{take|skip}_while] -------------------------------------------------------------");
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

    std::vector<int> zigzag =
	(c7::nseq::range(6, 0, 4)	// 0, 4, 8, 12, 16, 20
	 + c7::nseq::range(6, 21, -4)	// 21, 17, 13, 9, 5, 1
	 + c7::nseq::range(6, 2, 4)	// 2, 6, 10, 14, 18, 22
	 + c7::nseq::range(6, 23, -4))	// 23, 19, 15, 11, 7, 3
	| c7::nseq::to_vector();

    p_("\n[{drop|take}_tail_while] -------------------------------------------------------------");
    {
	p_("  zigzag              : %{}", zigzag);
	p_("  drop_tail_while(<12): %{}",
	   zigzag
	   | c7::nseq::drop_tail_while([](auto v){ return v < 12; }, 3)
	    );
	p_("  take_tail_while(<12): %{}",
	   zigzag
	   | c7::nseq::take_tail_while([](auto v){ return v < 12; }, 3)
	    );
    }
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_while();
}
