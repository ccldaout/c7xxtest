#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/enumerate.hpp>
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


[[maybe_unused]] static void test_enumerate_for_list()
{
    std::list<std::string> data{"open", "read", "write", "close"};

    p_("\n[enumerate for list] ------------------------------------------------");
    p_("  ls %{}", data);

    {
	auto ls = data;
	p_("  ls | enumerate(1)");
	p_("   -> %{}", ls | enumerate(1));
	p_("  ... and += :%%{i}");
	for (auto [i, a]: ls | enumerate(1)) {
	    P_("  %{} %{} -> ", i, a);
	    a += c7::format(":%{}", i);
	    p_("%{}", a);
	}
	p_("  ls %{}", ls);
    }

    {
	const auto& cls = data;
	p_("  cls | enumerate(9, -2");
	p_("   -> %{}", cls | enumerate(9, -2));
#if 0
	for (auto [i, a]: cls | enumerate(1)) {
	    P_("  %{} %{} -> ", i, a);
	    //a += c7::format(":%{}", i);	// COMPILE ERORR [OK]
	    p_("%{}", a);
	}
	p_("  ls %{}", cls);
#endif
    }
}


[[maybe_unused]] static void test_enumerate()
{
    c7::strvec data{"open", "read", "write", "close"};

    p_("\n[enumerate for vector] ----------------------------------------------");
    p_("  sv %{}", data);

    {
	auto sv = data;
	p_("  sv | enumerate(1)");
	p_("   -> %{}", sv | enumerate(1));
	p_("  ... and += :%%{i}");
	for (auto [i, a]: sv | enumerate(1)) {
	    P_("  %{} %{} -> ", i, a);
	    a += c7::format(":%{}", i);
	    p_("%{}", a);
	}
	p_("  sv %{}", sv);
    }

    {
	const auto& csv = data;
	p_("  csv | enumerate(9, -2)");
	p_("   -> %{}", csv | enumerate(9, -2));
#if 0
	for (auto [i, a]: csv | enumerate(1)) {
	    P_("  %{} %{} -> ", i, a);
	    //a += c7::format(":%{}", i);	// COMPILE ERORR [OK]
	    p_("%{}", a);
	}
	p_("  sv %{}", csv);
#endif
    }
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_enumerate();
    test_enumerate_for_list();
}
