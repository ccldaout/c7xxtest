#include <deque>
#include <unordered_set>
#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/insert.hpp>
#include <c7nseq/push.hpp>
#include <c7nseq/range.hpp>
#include <c7nseq/sort.hpp>
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


[[maybe_unused]] static void test_insert()
{
    std::vector<int> data{1, 5, 3, 5, 1, 1, 7, 3, 3, 2};

    p_("\n[insert] ------------------------------------------------------------");
    p_("  source %{}", data);
    {
	auto source = data;
	auto c = source | insert(std::unordered_set<int>{});
	p_("  source | insert(unordered_set{}) %{}",
	   std::is_reference_v<decltype(c)> ? "ref" : "val");
	p_("   -> %{}", c);

	p_("  source | insert(unordered_set{}) | to_vector | sort");
	p_("   -> %{}", c | to_vector() | sort());
	p_("  source %{}", source);
    }
    {
	auto source = data;
	std::unordered_set<int> cnt;
	auto& c = source | insert(cnt);
	p_("  source | insert(unordered_set&) %{}",
	   std::is_reference_v<decltype(c)> ? "ref" : "val");
	p_("   -> %{}", c);

	p_("  source | insert(unordered_set&) | to_vector | sort");
	p_("   -> %{}", c | to_vector() | sort());
	p_("  source %{}", source);
    }
}


[[maybe_unused]] static void test_push_front()
{
    p_("\n[push_front] --------------------------------------------------------");

    {
	auto c = range(5) | push_front(std::deque<int>{});
	p_("  range(5) | push_front(deque{}) %{}",
	   std::is_reference_v<decltype(c)> ? "ref" : "val");
	p_("   -> %{}", c);
    }

    {
	std::deque<int> cnt;
	auto& c = range(5) | push_front(cnt);
	p_("  range(5) | push_front(deque&) %{}",
	   std::is_reference_v<decltype(c)> ? "ref" : "val");
	p_("  <ret> %{}", c);
	p_("   cnt  %{}", cnt);
    }

    {
	auto c = range(5) | push_front(std::list<int>{});
	p_("  range(5) | push_front(list{}) %{}",
	   std::is_reference_v<decltype(c)> ? "ref" : "val");
	p_("   -> %{}", c);
    }

    {
	std::list<int> cnt;
	auto& c = range(5) | push_front(cnt);
	p_("  range(5) | push_front(list&) %{}",
	   std::is_reference_v<decltype(c)> ? "ref" : "val");
	p_("  <ret> %{}", c);
	p_("   cnt  %{}", cnt);
    }
}


[[maybe_unused]] static void test_push_back()
{
    p_("\n[push_back] ---------------------------------------------------------");

    {
	auto c = range(5) | push_back(std::vector<int>{});
	p_("  range(5) | push_back(vector{}) %{}",
	   std::is_reference_v<decltype(c)> ? "ref" : "val");
	p_("   -> %{}", c);
    }

    {
	std::vector<int> cnt;
	auto& c = range(5) | push_back(cnt);
	p_("  range(5) | push_back(vector&) %{}",
	   std::is_reference_v<decltype(c)> ? "ref" : "val");
	p_("  <ret> %{}", c);
	p_("   cnt  %{}", cnt);
    }

    {
	auto c = range(5) | push_back(std::list<int>{});
	p_("  range(5) | push_back(list{}) %{}",
	   std::is_reference_v<decltype(c)> ? "ref" : "val");
	p_("   -> %{}", c);
    }

    {
	std::list<int> cnt;
	auto& c = range(5) | push_back(cnt);
	p_("  range(5) | push_back(list&) %{}",
	   std::is_reference_v<decltype(c)> ? "ref" : "val");
	p_("  <ret> %{}", c);
	p_("   cnt  %{}", cnt);
    }
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_push_back();
    test_push_front();
    test_insert();
}
