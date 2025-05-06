#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/base64.hpp>
#include <c7nseq/c_array.hpp>
#include <c7nseq/string.hpp>
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


[[maybe_unused]] static void test_base64()
{
    p_("\n[base64] ------------------------------------------------------------");
    {
	std::string s{"abcdefghi"};
	auto e1 = c_array(s. c_str(), s. size()) | base64enc() | to_string();
	auto s1 = c_array(e1.c_str(), e1.size()) | base64dec() | to_string();
	p_("  %{} -> %{} -> '%{}'", s, e1, s1);

	s.pop_back();
	auto e2 = c_array(s. c_str(), s. size()) | base64enc() | to_string();
	auto s2 = c_array(e2.c_str(), e2.size()) | base64dec() | to_string();
	p_("  %{}  -> %{} -> '%{}'", s, e2, s2);

	s.pop_back();
	auto e3 = c_array(s. c_str(), s. size()) | base64enc() | to_string();
	auto s3 = c_array(e3.c_str(), e3.size()) | base64dec() | to_string();
	p_("  %{}   -> %{} -> '%{}'", s, e3, s3);
    }
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_base64();
}
