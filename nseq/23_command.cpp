#include <c7app.hpp>
#include <c7file.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/c_array.hpp>
#include <c7nseq/command.hpp>
#include <c7nseq/head.hpp>
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


static auto file_seq(const std::string& path)
{
    size_t z = 0;
    auto p = std::move(c7::file::mmap_r<char>(path, z).value());
    return c_array(std::move(p), z);
}


static void show_usedfd(int maxfd)
{
    struct stat stbuf;
    P_("used fd: ");
    for (int i = 0; i <= maxfd; i++) {
	if (::fstat(i, &stbuf) == C7_SYSOK) {
	    P_(" %{}", i);
	}
    }
    p_("");
}


[[maybe_unused]] static void test_command()
{
    p_("\n[command] ------------------------------------------------------------");
    {
	std::string data{"Hello World"};
	show_usedfd(20);
	p_("  string | command | head(8)");
	p_("   -> %{}", data | command<char>({"tr", "a-z", "A-Z"}, 3) | head(8));
	show_usedfd(20);
    }
    {
	show_usedfd(20);
	p_("  file | command | command | command");
	p_("   -> %{}",
	   file_seq("/etc/passwd")
	   | command<char>({"tr", "a-z", "A-Z"}, 8)
	   | command<char>({"cut", "-d:", "-f1,2"}, 9)
	   | command<char>({"head", "-n4"})
	    );
	show_usedfd(20);
    }
    {
	show_usedfd(20);
	p_("  file | command | command | command");
	p_("   -> %{}",
	   file_seq("/etc/passwd")
	   | command<char>(4)
	   . add({"tr", "a-z", "A-Z"})
	   . add("/bin/cut", {"cut", "-d:", "-f1,2"})
	   . add({"head", "-n4"})
	    );
	show_usedfd(20);
    }
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_command();
}
