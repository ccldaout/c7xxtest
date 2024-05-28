#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/range.hpp>
#include <c7nseq/reverse.hpp>
#include <c7nseq/slice.hpp>
#include <c7nseq/sort.hpp>
#include <c7nseq/transform.hpp>
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


[[maybe_unused]] static void test_slice()
{
    std::vector<int> data{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::list<int> data_ls{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    p_("\n[slice] -------------------------------------------------------------");

    // range
    {
	auto vec = range<int>(10);
	p_("  range(10) | slice(0, 3)");
	p_("   -> %{}", vec | slice(0, 3));
	p_("  range(10) | slice(1, 3)");
	p_("   -> %{}", vec | slice(1, 3));
	p_("  range(10) | slice(2, 3)");
	p_("   -> %{}", vec | slice(2, 3));
	p_("  range(10) | slice(1, 3, 2)");
	p_("   -> %{}", vec | slice(1, 3, 2));
	p_("  range(10) | slice(2, 3, 2)");
	p_("   -> %{}", vec | slice(2, 3, 2));
    }

    // vector

    {
	std::vector<int> vec{};
	p_("  vec{} | slice(0, 3)");
	p_("   -> %{}", vec | slice(0, 3));
	p_("  vec{} | slice(1, 3)");
	p_("   -> %{}", vec | slice(1, 3));
	p_("  vec{} | slice(2, 3)");
	p_("   -> %{}", vec | slice(2, 3));
	p_("  vec{} | slice(1, 3, 2)");
	p_("   -> %{}", vec | slice(1, 3, 2));
	p_("  vec{} | slice(2, 3, 2)");
	p_("   -> %{}", vec | slice(2, 3, 2));
    }

    {
	std::vector<int> vec{0};
	p_("  vec{0} | slice(0, 3)");
	p_("   -> %{}", vec | slice(0, 3));
	p_("  vec{0} | slice(1, 3)");
	p_("   -> %{}", vec | slice(1, 3));
	p_("  vec{0} | slice(2, 3)");
	p_("   -> %{}", vec | slice(2, 3));
	p_("  vec{0} | slice(1, 3, 2)");
	p_("   -> %{}", vec | slice(1, 3, 2));
	p_("  vec{0} | slice(2, 3, 2)");
	p_("   -> %{}", vec | slice(2, 3, 2));
    }

    {
	std::vector<int> vec{0,1};
	p_("  vec{0,1} | slice(0, 3)");
	p_("   -> %{}", vec | slice(0, 3));
	p_("  vec{0,1} | slice(1, 3)");
	p_("   -> %{}", vec | slice(1, 3));
	p_("  vec{0,1} | slice(2, 3)");
	p_("   -> %{}", vec | slice(2, 3));
	p_("  vec{0,1} | slice(1, 3, 2)");
	p_("   -> %{}", vec | slice(1, 3, 2));
	p_("  vec{0,1} | slice(2, 3, 2)");
	p_("   -> %{}", vec | slice(2, 3, 2));
    }

    {
	std::vector<int> vec{0,1,2};
	p_("  vec{0,1,2} | slice(0, 3)");
	p_("   -> %{}", vec | slice(0, 3));
	p_("  vec{0,1,2} | slice(1, 3)");
	p_("   -> %{}", vec | slice(1, 3));
	p_("  vec{0,1,2} | slice(2, 3)");
	p_("   -> %{}", vec | slice(2, 3));
	p_("  vec{0,1,2} | slice(1, 3, 2)");
	p_("   -> %{}", vec | slice(1, 3, 2));
	p_("  vec{0,1,2} | slice(2, 3, 2)");
	p_("   -> %{}", vec | slice(2, 3, 2));
    }

    p_("  vec %{}", data);
    {
	auto vec = data;
	p_("  vec | slice(0, 3)");
	p_("   -> %{}", vec | slice(0, 3));
	p_("  vec | slice(1, 3)");
	p_("   -> %{}", vec | slice(1, 3));
	p_("  vec | slice(2, 3)");
	p_("   -> %{}", vec | slice(2, 3));
	p_("  vec | slice(1, 3, 2)");
	p_("   -> %{}", vec | slice(1, 3, 2));
	p_("  vec | slice(2, 3, 2)");
	p_("   -> %{}", vec | slice(2, 3, 2));
    }

    // range & reverse
    {
	auto vec = range<int>(10);
	p_("  range(10) | slice(0, 3)   | reverse");
	p_("   -> %{}", vec | slice(0, 3) | reverse());
	p_("  range(10) | slice(1, 3)   | reverse");
	p_("   -> %{}", vec | slice(1, 3) | reverse());
	p_("  range(10) | slice(2, 3)   | reverse");
	p_("   -> %{}", vec | slice(2, 3) | reverse());
	p_("  range(10) | slice(1, 3, 2)| reverse");
	p_("   -> %{}", vec | slice(1, 3, 2) | reverse());
	p_("  range(10) | slice(2, 3, 2)| reverse");
	p_("   -> %{}", vec | slice(2, 3, 2) | reverse());
    }

    // vector & reverse

    {
	std::vector<int> vec{};
	p_("  vec{} | slice(0, 3)   | reverse");
	p_("   -> %{}", vec | slice(0, 3) | reverse());
	p_("  vec{} | slice(1, 3)   | reverse");
	p_("   -> %{}", vec | slice(1, 3) | reverse());
	p_("  vec{} | slice(2, 3)   | reverse");
	p_("   -> %{}", vec | slice(2, 3) | reverse());
	p_("  vec{} | slice(1, 3, 2)| reverse");
	p_("   -> %{}", vec | slice(1, 3, 2) | reverse());
	p_("  vec{} | slice(2, 3, 2)| reverse");
	p_("   -> %{}", vec | slice(2, 3, 2) | reverse());
    }

    {
	std::vector<int> vec{0};
	p_("  vec{0} | slice(0, 3)   | reverse");
	p_("   -> %{}", vec | slice(0, 3) | reverse());
	p_("  vec{0} | slice(1, 3)   | reverse");
	p_("   -> %{}", vec | slice(1, 3) | reverse());
	p_("  vec{0} | slice(2, 3)   | reverse");
	p_("   -> %{}", vec | slice(2, 3) | reverse());
	p_("  vec{0} | slice(1, 3, 2)| reverse");
	p_("   -> %{}", vec | slice(1, 3, 2) | reverse());
	p_("  vec{0} | slice(2, 3, 2)| reverse");
	p_("   -> %{}", vec | slice(2, 3, 2) | reverse());
    }

    {
	std::vector<int> vec{0,1};
	p_("  vec{0,1} | slice(0, 3)   | reverse");
	p_("   -> %{}", vec | slice(0, 3) | reverse());
	p_("  vec{0,1} | slice(1, 3)   | reverse");
	p_("   -> %{}", vec | slice(1, 3) | reverse());
	p_("  vec{0,1} | slice(2, 3)   | reverse");
	p_("   -> %{}", vec | slice(2, 3) | reverse());
	p_("  vec{0,1} | slice(1, 3, 2)| reverse");
	p_("   -> %{}", vec | slice(1, 3, 2) | reverse());
	p_("  vec{0,1} | slice(2, 3, 2)| reverse");
	p_("   -> %{}", vec | slice(2, 3, 2) | reverse());
    }

    {
	std::vector<int> vec{0,1,2};
	p_("  vec{0,1,2} | slice(0, 3)   | reverse");
	p_("   -> %{}", vec | slice(0, 3) | reverse());
	p_("  vec{0,1,2} | slice(1, 3)   | reverse");
	p_("   -> %{}", vec | slice(1, 3) | reverse());
	p_("  vec{0,1,2} | slice(2, 3)   | reverse");
	p_("   -> %{}", vec | slice(2, 3) | reverse());
	p_("  vec{0,1,2} | slice(1, 3, 2)| reverse");
	p_("   -> %{}", vec | slice(1, 3, 2) | reverse());
	p_("  vec{0,1,2} | slice(2, 3, 2)| reverse");
	p_("   -> %{}", vec | slice(2, 3, 2) | reverse());
    }

    {
	auto vec = data;
	p_("  vec | slice(0, 3)   | reverse");
	p_("   -> %{}", vec | slice(0, 3) | reverse());
	p_("  vec | slice(1, 3)   | reverse");
	p_("   -> %{}", vec | slice(1, 3) | reverse());
	p_("  vec | slice(2, 3)   | reverse");
	p_("   -> %{}", vec | slice(2, 3) | reverse());
	p_("  vec | slice(1, 3, 2)| reverse");
	p_("   -> %{}", vec | slice(1, 3, 2) | reverse());
	p_("  vec | slice(2, 3, 2)| reverse");
	p_("   -> %{}", vec | slice(2, 3, 2) | reverse());
    }

    // list

    {
	std::list<int> ls{};
	p_("  list{} | slice(0, 3)");
	p_("   -> %{}", ls | slice(0, 3));
	p_("  list{} | slice(1, 3)");
	p_("   -> %{}", ls | slice(1, 3));
	p_("  list{} | slice(2, 3)");
	p_("   -> %{}", ls | slice(2, 3));
	p_("  list{} | slice(1, 3, 2)");
	p_("   -> %{}", ls | slice(1, 3, 2));
	p_("  list{} | slice(2, 3, 2)");
	p_("   -> %{}", ls | slice(2, 3, 2));
    }

    {
	std::list<int> ls{0};
	p_("  list{0} | slice(0, 3)");
	p_("   -> %{}", ls | slice(0, 3));
	p_("  list{0} | slice(1, 3)");
	p_("   -> %{}", ls | slice(1, 3));
	p_("  list{0} | slice(2, 3)");
	p_("   -> %{}", ls | slice(2, 3));
	p_("  list{0} | slice(1, 3, 2)");
	p_("   -> %{}", ls | slice(1, 3, 2));
	p_("  list{0} | slice(2, 3, 2)");
	p_("   -> %{}", ls | slice(2, 3, 2));
    }

    {
	std::list<int> ls{0,1};
	p_("  list{0,1} | slice(0, 3)");
	p_("   -> %{}", ls | slice(0, 3));
	p_("  list{0,1} | slice(1, 3)");
	p_("   -> %{}", ls | slice(1, 3));
	p_("  list{0,1} | slice(2, 3)");
	p_("   -> %{}", ls | slice(2, 3));
	p_("  list{0,1} | slice(1, 3, 2)");
	p_("   -> %{}", ls | slice(1, 3, 2));
	p_("  list{0,1} | slice(2, 3, 2)");
	p_("   -> %{}", ls | slice(2, 3, 2));
    }

    {
	std::list<int> ls{0,1,2};
	p_("  list{0,1,2} | slice(0, 3)");
	p_("   -> %{}", ls | slice(0, 3));
	p_("  list{0,1,2} | slice(1, 3)");
	p_("   -> %{}", ls | slice(1, 3));
	p_("  list{0,1,2} | slice(2, 3)");
	p_("   -> %{}", ls | slice(2, 3));
	p_("  list{0,1,2} | slice(1, 3, 2)");
	p_("   -> %{}", ls | slice(1, 3, 2));
	p_("  list{0,1,2} | slice(2, 3, 2)");
	p_("   -> %{}", ls | slice(2, 3, 2));
    }

    {
	auto ls = data_ls;
	p_("  list | slice(0, 3)");
	p_("   -> %{}", ls | slice(0, 3));
	p_("  list | slice(1, 3)");
	p_("   -> %{}", ls | slice(1, 3));
	p_("  list | slice(2, 3)");
	p_("   -> %{}", ls | slice(2, 3));
	p_("  list | slice(1, 3, 2)");
	p_("   -> %{}", ls | slice(1, 3, 2));
	p_("  list | slice(2, 3, 2");
	p_("   -> %{}", ls | slice(2, 3, 2));
    }

    // list & reverse

    {
	std::list<int> ls{};
	p_("  list{} | slice(0, 3)   | reverse");
	p_("   -> %{}", ls | slice(0, 3) | reverse());
	p_("  list{} | slice(1, 3)   | reverse");
	p_("   -> %{}", ls | slice(1, 3) | reverse());
	p_("  list{} | slice(2, 3)   | reverse");
	p_("   -> %{}", ls | slice(2, 3) | reverse());
	p_("  list{} | slice(1, 3, 2)| reverse");
	p_("   -> %{}", ls | slice(1, 3, 2) | reverse());
	p_("  list{} | slice(2, 3, 2)| reverse");
	p_("   -> %{}", ls | slice(2, 3, 2) | reverse());
    }

    {
	std::list<int> ls{0};
	p_("  list{0} | slice(0, 3)   | reverse");
	p_("   -> %{}", ls | slice(0, 3) | reverse());
	p_("  list{0} | slice(1, 3)   | reverse");
	p_("   -> %{}", ls | slice(1, 3) | reverse());
	p_("  list{0} | slice(2, 3)   | reverse");
	p_("   -> %{}", ls | slice(2, 3) | reverse());
	p_("  list{0} | slice(1, 3, 2)| reverse");
	p_("   -> %{}", ls | slice(1, 3, 2) | reverse());
	p_("  list{0} | slice(2, 3, 2)| reverse");
	p_("   -> %{}", ls | slice(2, 3, 2) | reverse());
    }

    {
	std::list<int> ls{0,1};
	p_("  list{0,1} | slice(0, 3)   | reverse");
	p_("   -> %{}", ls | slice(0, 3) | reverse());
	p_("  list{0,1} | slice(1, 3)   | reverse");
	p_("   -> %{}", ls | slice(1, 3) | reverse());
	p_("  list{0,1} | slice(2, 3)   | reverse");
	p_("   -> %{}", ls | slice(2, 3) | reverse());
	p_("  list{0,1} | slice(1, 3, 2)| reverse");
	p_("   -> %{}", ls | slice(1, 3, 2) | reverse());
	p_("  list{0,1} | slice(2, 3, 2)| reverse");
	p_("   -> %{}", ls | slice(2, 3, 2) | reverse());
    }

    {
	std::list<int> ls{0,1,2};
	p_("  list{0,1,2} | slice(0, 3)   | reverse");
	p_("   -> %{}", ls | slice(0, 3) | reverse());
	p_("  list{0,1,2} | slice(1, 3)   | reverse");
	p_("   -> %{}", ls | slice(1, 3) | reverse());
	p_("  list{0,1,2} | slice(2, 3)   | reverse");
	p_("   -> %{}", ls | slice(2, 3) | reverse());
	p_("  list{0,1,2} | slice(1, 3, 2)| reverse");
	p_("   -> %{}", ls | slice(1, 3, 2) | reverse());
	p_("  list{0,1,2} | slice(2, 3, 2)| reverse");
	p_("   -> %{}", ls | slice(2, 3, 2) | reverse());
    }

    {
	auto ls = data_ls;
	p_("  list | slice(0, 3)   | reverse");
	p_("   -> %{}", ls | slice(0, 3) | reverse());
	p_("  list | slice(1, 3)   | reverse");
	p_("   -> %{}", ls | slice(1, 3) | reverse());
	p_("  list | slice(2, 3)   | reverse");
	p_("   -> %{}", ls | slice(2, 3) | reverse());
	p_("  list | slice(1, 3, 2)| reverse");
	p_("   -> %{}", ls | slice(1, 3, 2) | reverse());
	p_("  list | slice(2, 3, 2)| reverse");
	p_("   -> %{}", ls | slice(2, 3, 2) | reverse());
    }

    {
	auto ls = data_ls;
	p_("  list | slice(1,3) | transform(x100)");
	p_("   -> %{}", ls | slice(1, 3) | transform([](auto& v) { return (v *= 100); }));
	p_("  ls %{}", ls);
    }

    {
	auto vec = data;
	p_("  vec | slice(2,3) | sort_by(>)");
	p_("   -> %{}", vec | slice(2, 3) | sort_by(std::greater<int>{}));
	p_("  vec %{}", vec);
    }
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_slice();
}
