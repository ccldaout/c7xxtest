#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/tuple.hpp>
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


[[maybe_unused]] static void test_tuple()
{
    p_("\n[tuple] -------------------------------------------------------------");
    {
	std::tuple<int, std::string, double> data{1, "one", 0.1};
	p_("  std::apply -> %{}",
	   std::apply([](auto i, const auto& s, auto d) {
			  return c7::format("i:%{}, s:%{}, d:%{}", i, s, d);
		      }, data));
	tuple_for_each(data, [](auto& v) {
				 p_("  tuple_for_each: v:%{}", v);
			  });
	p_("  tuple_transform -> %{}",
	   tuple_transform(data, [](auto& v) {
				     return &v;
				 }));
    }
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_tuple();
}
