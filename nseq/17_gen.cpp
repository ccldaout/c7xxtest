#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/generator.hpp>
#include <c7nseq/head.hpp>
#include <c7nseq/range.hpp>
#include <c7nseq/string.hpp>
#include <c7nseq/vector.hpp>
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


[[maybe_unused]] static void test_generator()
{
    p_("\n[generator] ---------------------------------------------------------");
    {
	auto source =
	    generator<int>(
		[](auto& out) {
		    for (int i = 5; i < 9; i++) {
			out << i;
		    }
		}, 4);
	auto twice =
	    generator<int>(
		[](auto& seq, auto& out) {
		    for (auto v: seq) {
			out << v * 2;
		    }
		}, 3);
	p_("  generator | generator(twice)");
	p_("   -> %{}", source() | twice);
    }
    {
	auto g_pipe =
	    generator<int>(
		[](auto& seq, auto& out) {
		    auto it = std::begin(seq);
		    for (int n = 1; n <= 4; n++) {
			int v = 0;
			for (int i = 0; i < n; i++) {
			    v += *it;
			    ++it;
			}
			out << v;
		    }
		}, 5)
	    | generator<std::string>(
		[](auto& seq, auto& out) {
		    for (auto i: seq) {
			out << c7::format("str<%{}>", i);
		    }
		}, 4);
	p_("  range(10) | generator | generator");
	p_("   -> %{}", range(10) | g_pipe);
	p_("  range(10, 100, 0) | generator | generator");
	p_("   -> %{}", range(10, 100, 0) | g_pipe);
    }

    {
	c7::strvec kws{ "zero", "one", "two", "three", "four", "five", "six", "seven" };
	auto proc =
	    generator<std::string>(
		[&kws](auto& seq, auto& out) mutable {
		    for (auto& s: kws) {
			out << std::move(s);
		    }
		}, 5);
	p_("  kws | generator | head(3) | to_vector");
	p_("   -> %{}", kws | proc | head(3) | to_vector());
	p_("  kwd (after) %{}", kws);

    }

    {
	c7::strvec kws{ "zero", "one", "two", "three", "four", "five", "six", "seven" };
	const auto& ckws = kws;
	auto proc =
	    generator<std::string>(
		[](auto& seq, auto& out) mutable {
		    for (auto& s: seq) {
			out << c7::str::transpose(s, [](auto c){return std::toupper(c);});
		    }
		});
	p_("  ckws | generator | head(3) | to_vector");
	p_("   -> %{}", ckws | proc | head(3) | to_vector());
    }

    {
	c7::strvec kws{ "zero", "one", "two", "three", "four", "five", "six", "seven" };
	auto join =
	    generator<char>(
		[](auto& seq, auto& out) {
		    for (auto& s: seq) {
			out << s;
			out << '+';
		    }
		});
	p_("  kws | genetor | to_string");
	p_("   -> %{}", kws | join | to_string());
    }
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_generator();
}
