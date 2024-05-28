#include <c7app.hpp>
#include <c7defer.hpp>
#include <c7delegate.hpp>
#include <c7fd.hpp>
#include <c7proc.hpp>
#include <c7thread.hpp>
#include <c7result.hpp>
#include <deque>
#include <list>
#include <vector>
#include <unordered_map>


template <typename T>
static void show(const char *s)
{
    c7::p_("%{} %{}", s, sizeof(T));
}


#define SHOW(T)		show<T>(#T)


int main()
{
    SHOW(std::vector<int>);
    SHOW(std::deque<int>);
    SHOW(std::list<int>);
    SHOW(std::function<void()>);
    using map_string_int = std::unordered_map<std::string, int>;
    SHOW(map_string_int);
    SHOW(c7::defer);
    SHOW(c7::delegate<void>);
    SHOW(c7::fd);
    SHOW(c7::proc);
    SHOW(c7::thread::mutex);
    SHOW(c7::thread::condvar);
    SHOW(c7::thread::thread);
    SHOW(c7::result<>);
    SHOW(c7::result<long>);
}
