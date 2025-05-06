#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/generator.hpp>
#include <c7nseq/flat.hpp>
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


[[maybe_unused]] static void test_flat()
{
    p_("\n[flat] --------------------------------------------------------------");
    {
	auto source =
	    generator<std::vector<int>>(
		[](auto& out) {
		    out << std::vector<int>{1, 2, 3};
		    out << std::vector<int>{4, 5, 6};
		    out << std::vector<int>{7, 8, 9};
		});
	p_("  source");
	p_("   -> %{}", source());
	p_("  source | flat<0>");
	p_("   -> %{}", source() | flat<0>());
    }
    {
	auto source =
	    generator<std::vector<const char *>>(
		[](auto& out) {
		    out << std::vector<const char *>{"one", "two", "three"};
		    out << std::vector<const char *>{"four", "five", "six"};
		    out << std::vector<const char *>{"seven", "eight", "nine"};
		});
	p_("  source");
	p_("   -> %{}", source());
	p_("  source | flat<0>");
	p_("   -> %{}", source() | flat<0>());
    }
    {
	auto source =
	    generator<std::vector<std::string>>(
		[](auto& out) {
		    out << std::vector<std::string>{"one", "two", "three"};
		    out << std::vector<std::string>{"four", "five", "six"};
		    out << std::vector<std::string>{"seven", "eight", "nine"};
		});
	p_("  source");
	p_("   -> %{}", source());
	p_("  source | flat<0>");
	p_("   -> %{}", source() | flat<0>());
    }
    {
	auto source =
	    generator<std::vector<std::string>>(
		[](auto& out) {
		    out << std::vector<std::string>{"one", "two", "three"};
		    out << std::vector<std::string>{"four", "five", "six"};
		    out << std::vector<std::string>{"seven", "eight", "nine"};
		});
	p_("  source");
	p_("   -> %{}", source());
	p_("  source | flat<1>");
	p_("   -> %{}", source() | flat<1>());
    }
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_flat();
}
