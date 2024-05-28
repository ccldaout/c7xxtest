#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/map.hpp>
#include <c7nseq/range.hpp>
#include <c7nseq/set.hpp>
#include <c7nseq/sort.hpp>
#include <c7nseq/vector.hpp>
#include <c7nseq/zip.hpp>
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


[[maybe_unused]] static void test_to_map()
{
    c7::strvec data{"zero", "one", "two", "three", "four", "five", "six", "seven"};

    p_("\n[map] ---------------------------------------------------------------");
    p_("  kws %{}", data);
    {
	auto kws = data;
	auto c = zip(range(8), kws) | to_map();
	p_("  zip(range(8), kws) | to_map %{}",
	   std::is_reference_v<decltype(c)> ? "ref" : "val");
	p_("   -> %{}", c);

	p_("  zip(range(8), kws) | to_map | to_vector | sort");
	p_("   -> %{}", c | to_vector() | sort());
	p_("  kws %{}", kws);
    }
    {
	auto kws = data;
	auto c = zip(kws, range(8)) | to_map();
	p_("  zip(kws, range(8)) | to_map %{}",
	   std::is_reference_v<decltype(c)> ? "ref" : "val");
	p_("   -> %{}", c);

	p_("  zip(kws, range(8)) | to_map | to_vector | sort");
	p_("   -> %{}", c | to_vector() | sort());
	p_("  kws %{}", kws);
    }
}


[[maybe_unused]] static void test_to_set()
{
    std::vector<int> data{1, 5, 3, 5, 1, 1, 7, 3, 3, 2};

    p_("\n[set] ---------------------------------------------------------------");
    p_("  source %{}", data);
    {
	auto source = data;
	auto c = source | to_set();
	p_("  source | to_set %{}",
	   std::is_reference_v<decltype(c)> ? "ref" : "val");
	p_("   -> %{}", c);

	p_("  source | to_set | to_vector | sort");
	p_("   -> %{}", c | to_vector() | sort());
	p_("  source %{}", source);
    }
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_to_set();
    test_to_map();
}
