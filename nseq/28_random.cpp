#include <c7app.hpp>
#include <c7format/helper.hpp>
#include <c7path.hpp>
#include <c7string.hpp>
#include <c7mlog.hpp>
#include <c7nseq/head.hpp>
#include <c7nseq/random.hpp>


using namespace c7::nseq;


c7::mlog_writer mlog;

#define mlog(_lv, ...)	\
    mlog.format(__FILE__, __LINE__, C7_LOG_##_lv, 0, 0, __VA_ARGS__)

using c7::p_;
using c7::P_;

[[maybe_unused]] static void break_point()
{
}


[[maybe_unused]] static void test_random()
{
    p_("\n[test_random] -----------------------------------------------------------");

    p_("%{}", random_uniform_dist(1234, 0, 9) | head(20));
    p_("%{}", random_uniform_dist(1234, 0.0, 1.0) | head(20));
    p_("%{}", random_bernoulli_dist(1234, 0.4) | head(20));
    p_("%{}", random_binomial_dist(1234, 30, 0.7) | head(20));
    p_("%{}", random_lognormal_dist(1234, 170.0, 5.0) | head(20));
    p_("%{}", random_poisson_dist<int>(1234, 0.9) | head(20));
    p_("%{}", random_normal_dist(1234, 170.0, 5.0) | head(20));
}


int main(int argc, char **)
{
    //auto flags = C7_MLOG_F_THREAD_NAME|C7_MLOG_F_SOURCE_NAME;
    //mlog.init("./N", 0, 8192*1024, flags);

    test_random();
}
