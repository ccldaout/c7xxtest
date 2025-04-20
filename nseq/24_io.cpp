#include <fstream>
#include <c7app.hpp>
#include <c7fd.hpp>
#include <c7path.hpp>
#include <c7string.hpp>
#include <c7mlog.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/io.hpp>
#include <c7nseq/string.hpp>
#include <c7nseq/transform.hpp>


using namespace c7::nseq;


c7::mlog_writer mlog;

#define mlog(_lv, ...)	\
    mlog.format(__FILE__, __LINE__, C7_LOG_##_lv, 0, 0, __VA_ARGS__)

using c7::p_;
using c7::P_;

[[maybe_unused]] static void break_point()
{
}


[[maybe_unused]] static void mktest(const std::string& path)
{
    std::string text =
	"int main()\n"
	"{\n"
	"    return 0;\n"
	"}\n";
    c7::file::write(path, 0644, text.c_str(), text.size()).check();
}


[[maybe_unused]] static void test_io()
{
    p_("\n[test_io] -----------------------------------------------------------");

    auto path = c7::path::untildize("~/tmp/nseq.txt");
    mktest(path);

    {
	auto ns
	    = mmap_r<char>(path)
	    | transform([](auto c) { return std::toupper(c); });
	p_("  mmap_r | transform -> <%{}>", ns | to_string());
    }
    {
	auto home = c7::path::untildize("~");
	auto dirfd = std::move(c7::open(home).value());
	auto ns
	    = mmap_r<char>(dirfd, "tmp/nseq.txt")
	    | transform([](auto c) { return std::toupper(c); });
	p_("  mmap_r(dirfd) | transform -> <%{}>", ns | to_string());
    }
    {
	auto ns
	    = istream<char>(std::ifstream(path))
	    | transform([](auto c) { return std::toupper(c); });
	p_("  istream | transform -> <%{}>", ns | to_string());
    }
    {
	auto ns
	    = istream<char>(std::ifstream(path), false)
	    | transform([](auto c) { return std::toupper(c); });
	p_("  istream(skipws) | transform -> <%{}>", ns | to_string());
    }

    {
	auto path = c7::path::untildize("~/tmp/nseq.empty");
	c7::file::write(path, 0644, nullptr, 0).check();
	auto ns
	    = mmap_r<char>(path)
	    | transform([](auto c) { return std::toupper(c); });
	p_("  mmap_r(empty_file) | transform -> <%{}>", ns | to_string());
    }
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_io();
}
