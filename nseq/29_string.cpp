#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7path.hpp>
#include <c7string.hpp>
#include <c7mlog.hpp>
#include <c7nseq/string.hpp>


using namespace c7::nseq;


c7::mlog_writer mlog;

#define mlog(_lv, ...)	\
    mlog.format(__FILE__, __LINE__, C7_LOG_##_lv, 0, 0, __VA_ARGS__)

using c7::p_;
using c7::P_;

[[maybe_unused]] static void break_point()
{
}


[[maybe_unused]] static void test_string()
{
    p_("\n[test_string] -----------------------------------------------------------");

    {
	std::string s = " \t\f\n string \t\f\n";
	auto s2 = s | trim() | to_string();
	p_("<%{}> -> <%{}>", s, s2);
    }
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_string();
}
