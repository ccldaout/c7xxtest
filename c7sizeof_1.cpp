#include <c7app.hpp>
#include <c7defer.hpp>
#include <c7delegate.hpp>
#include <c7fd.hpp>
#include <c7json.hpp>
#include <c7proc.hpp>
#include <c7thread.hpp>
#include <c7result.hpp>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <unordered_map>
#include <unordered_set>


template <typename T>
static void show(const char *s)
{
    c7::p_("%{} %{}", s, sizeof(T));
}


#define SHOW(T)		show<T>(#T)


using map_str_int = std::map<std::string, int>;
using unordered_map_str_int = std::unordered_map<std::string, int>;


int main()
{
    SHOW(std::string);
    SHOW(std::istringstream);
    SHOW(std::ostringstream);
    SHOW(std::vector<int>);
    SHOW(std::deque<int>);
    SHOW(std::list<int>);
    SHOW(map_str_int);
    SHOW(unordered_map_str_int);
    SHOW(std::set<std::string>);
    SHOW(std::unordered_set<std::string>);
    SHOW(std::function<void()>);
    SHOW(c7::defer);
    SHOW(c7::delegate<void>);
    SHOW(c7::fd);
    SHOW(c7::proc);
    SHOW(c7::thread::mutex);
    SHOW(c7::thread::condvar);
    SHOW(c7::thread::thread);
    SHOW(c7::result<>);
    SHOW(c7::result<long>);
    SHOW(c7::json_object);
    SHOW(c7::json_struct);
}
