#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7path.hpp>
#include <c7string.hpp>
#include <c7mlog.hpp>
#include <c7nseq.hpp>


using namespace c7::nseq;


c7::mlog_writer mlog;

#define mlog(_lv, ...)	\
    mlog.format(__FILE__, __LINE__, C7_LOG_##_lv, 0, 0, __VA_ARGS__)

using c7::p_;
using c7::P_;

[[maybe_unused]] static void break_point()
{
}


template <typename T>
static void show(const vseq<T>& vs)
{
    for (auto&& v: vs) {
	P_(" <%{}> @%{}", v, &v);
    }
    p_("");
}


[[maybe_unused]] static void test_vseq()
{
    p_("\n[test_vseq] -----------------------------------------------------------");

    p_("[range]");
    show<int>(c7::nseq::range(5));
    show<int>(c7::nseq::range(5) | c7::nseq::reverse());

    std::vector<int> iv{0, 1, 2, 3, 4, 5};
    p_("[vector] addr: %{}", iv.data());
    p_("int&");
    show<int&>(iv);
    show<int&>(iv | c7::nseq::filter([](auto&& v){ return v%2==0; }));
    show<int&>(iv | c7::nseq::reverse());
    p_("int");
    show<int>(iv);
    show<int>(iv | c7::nseq::filter([](auto&& v){ return v%2==0; }));
    show<int>(iv | c7::nseq::reverse());
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_vseq();
}
