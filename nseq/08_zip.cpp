#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/range.hpp>
#include <c7nseq/sort.hpp>
#include <c7nseq/transform.hpp>
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


[[maybe_unused]] static void test_zipN()
{
    c7::strvec data{"zero", "one", "two", "three", "four", "five", "six", "seven"};

    p_("\n[zipN] --------------------------------------------------------------");
    p_("  kws %{}", data);
    {
	auto kws = data;
	auto c = zip(range(10), kws, range<double>(5, 0.3, 0.1));
	p_("  zip(range(10), kws, range<double>(5, 0.3, 0.1)) %{}",
	   std::is_reference_v<decltype(c)> ? "ref" : "val");
	p_("   -> %{}", c);
    }

    {
	auto kws = data;
	auto c = zip(kws, range(6), range<double>(5, 0.3, 0.1));
	p_("  zip(kws, range(6), range<double>(5, 0.3, 0.1)) %{}",
	   std::is_reference_v<decltype(c)> ? "ref" : "val");
	p_("   -> %{}", c);

	p_("  zip(kws, range(6), range<double>(5, 0.3, 0.1)) | to_vector | sort");
	p_("   -> %{}", c | to_vector() | sort());
	p_("  kws %{}", kws);
    }

    {
	auto kws = data;
	auto c = zip(kws | ptr(), range(6), range<double>(5, 0.3, 0.1));
	p_("  zip(kws|ptr, range(6), range<double>(5, 0.3, 0.1)) %{}",
	   std::is_reference_v<decltype(c)> ? "ref" : "val");
	p_("   -> %{}", c);

	p_("  zip(kws|ptr, range(6), range<double>(5, 0.3, 0.1)) | to_vector | sort");
	p_("   -> %{}", c | to_vector() | sort());
	p_("  kws %{}", kws);
    }
}


[[maybe_unused]] static void test_zip()
{
    c7::strvec data{"zero", "one", "two", "three", "four", "five", "six", "seven"};

    p_("\n[zip2] --------------------------------------------------------------");
    p_("  kws %{}", data);
    {
	auto kws = data;
	auto c = zip(range(10), kws);
	p_("  zip(range(10), kws) %{}",
	   std::is_reference_v<decltype(c)> ? "ref" : "val");
	p_("   -> %{}", c);
    }

    {
	auto kws = data;
	auto c = zip(kws, range(6));
	p_("  zip(kws, range(6)) %{}",
	   std::is_reference_v<decltype(c)> ? "ref" : "val");
	p_("   -> %{}", c);

	p_("  zip(kws, range(6)) | to_vector | sort");
	p_("   -> %{}", c | to_vector() | sort());
	p_("  kws %{}", kws);
    }

    {
	auto kws = data;
	auto c = zip(kws | ptr(), range(6));
	p_("  zip(kws|ptr, range(6)) %{}",
	   std::is_reference_v<decltype(c)> ? "ref" : "val");
	p_("   -> %{}", c);

	p_("  zip(kws|ptr, range(6)) | to_vector | sort");
	p_("   -> %{}", c | to_vector() | sort());
	p_("  kws %{}", kws);
    }
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_zip();
    test_zipN();
}
