#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/filter.hpp>
#include <c7nseq/transform.hpp>
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


[[maybe_unused]] static void test_basic_for_list()
{
    std::list<int> ls{1, 2, 3, 4, 5};

    auto conv = [](auto& v) { return v * 3; };
    auto pred = [](auto v) { return v % 2 == 0; };

    p_("\n[basic for list] ----------------------------------------------------");

    p_("  ls | tarnsform(*3) | filter(even)");
    p_("   -> %{}",
       ls | transform(conv) | filter(pred));

    auto& ref{ls};
    p_("  &ls | tarnsform(*3) | filter(even)");
    p_("   -> %{}",
       ref | transform(conv) | filter(pred));

    auto pipe_rtf = ref | transform(conv) | filter(pred);
    p_("  auro pipe_rtf = ref | tarnsform(*3) | filter(even)");
    p_("   -> %{}",
       pipe_rtf);

    auto cpipe_tf = transform(conv) | filter(pred);
    p_("  ref | (auto cpipe_tf = transport(*3) | filter(even))");
    p_("   -> %{}", ref | cpipe_tf);

    const std::list<int> cls{1, 2, 3, 4, 5};
    p_("  cls | tarnsform(*3) | filter(even)");
    p_("   -> %{}",
       cls | transform(conv) | filter(pred));

    const auto& cref{ls};
    p_("  const &ls | tarnsform(*3) | filter(even)");
    p_("   -> %{}",
       cref | transform(conv) | filter(pred));

    p_("  ls | filter(even) | transform(*=100)");
    p_("   -> %{}",
       ls | filter(pred) | transform([](auto& v) { v *= 100; return v; }));
    p_("  ls");
    p_("   -> %{}", ls);

    p_("  std::move(ls) | transform(*3) | filter(event)");
    p_("   -> %{}", std::move(ls) | transform(conv) | filter(pred));
    p_("  ls");
    p_("   -> %{}", ls);
}


[[maybe_unused]] static void test_basic()
{
    std::vector<int> vec{1, 2, 3, 4, 5};

    auto conv = [](auto& v) { return v * 3; };
    auto pred = [](auto v) { return v % 2 == 0; };

    p_("\n[basic for vector] --------------------------------------------------");

    p_("  vec | tarnsform(*3) | filter(even)");
    p_("   -> %{}",
       vec | transform(conv) | filter(pred));

    auto& ref{vec};
    p_("  ref | tarnsform(*3) | filter(even)");
    p_("   -> %{}",
       ref | transform(conv) | filter(pred));

    auto pipe_rtf = ref | transform(conv) | filter(pred);
    p_("  auto pipe_rtf = ref | tarnsform(*3) | filter(even)");
    p_("   -> %{}",
       pipe_rtf);

    auto cpipe_tf = transform(conv) | filter(pred);
    p_("  ref | (auto cpipe_tf = transport(*3) | filter(even))");
    p_("   -> %{}", ref | cpipe_tf);

    const std::list<int> cvec{1, 2, 3, 4, 5};
    p_("  cvec | tarnsform(*3) | filter(even)");
    p_("   -> %{}",
       cvec | transform(conv) | filter(pred));

    const auto& cref{vec};
    p_("  const &vec | tarnsform(*3) | filter(even)");
    p_("   -> %{}",
       cref | transform(conv) | filter(pred));

    p_("  vec | filter(even) | transform(*=100)");
    p_("   -> %{}",
       vec | filter(pred) | transform([](auto& v) { v *= 100; return v; }));
    p_("  vec");
    p_("   -> %{}", vec);

    p_("  std::move(vec) | transform(*3) | filter(event)");
    p_("   -> %{}", std::move(vec) | transform(conv) | filter(pred));
    p_("  vec");
    p_("   -> %{}", vec);
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_basic();
    test_basic_for_list();
}
