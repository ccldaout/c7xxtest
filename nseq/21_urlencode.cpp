#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/base64.hpp>
#include <c7nseq/c_array.hpp>
#include <c7nseq/group.hpp>
#include <c7nseq/string.hpp>
#include <c7nseq/urlencode.hpp>
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


[[maybe_unused]] static void test_urlencode()
{
    p_("\n[urlencode] ----------------------------------------------------------");
    {
	std::string s{"<oops>-_.~ name</oops>"};
	auto e1 = c_array(s. c_str(), s. size()) | urlencode_url() | to_string();
	auto d1 = c_array(e1.c_str(), e1.size()) | urldecode()     | to_string();
	p_("  %{} -> '%{}' -> '%{}'", s, e1, d1);

	auto e2 = c_array(s. c_str(), s. size()) | urlencode_body() | to_string();
	auto d2 = c_array(e2.c_str(), e2.size()) | urldecode()      | to_string();
	p_("  %{} -> '%{}' -> '%{}'", s, e2, d2);
    }
    {
	std::string s{"int main() { std::cout << \"Oops!\" << std::endl; }"};
	int n = 0;
	auto ws = c_array(s.c_str(), s.size())
	    | base64enc()
	    | urlencode_body()
	    | group_by([&n](const auto&, const auto&) mutable {
			   if (n++ < 16) {
			       return true;
			   } else {
			       n = 0;
			       return false;
			   }
		       });
	p_("  c_array | base64enc | urlencode_body | group_by");
	P_("   ->");
	for (auto&& w: ws) {
	    P_(" %{}", w | to_string());
	}
	p_("");
    }
    {
	p_("<%{}>", "JBX-01" | urlencode_body() | to_string());
    }
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_urlencode();
}
