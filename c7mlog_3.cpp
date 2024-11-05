#include <c7app.hpp>
#include <c7mlog.hpp>
#include <c7thread.hpp>
#include <thread>


#define mlog(g, ...)	c7::mlog.format(__FILE__, __LINE__, C7_LOG_##g, 0, 0, __VA_ARGS__)


static void thread_c7()
{
    mlog(ALW, "thread_c7 #1");

    c7::thread::thread th2;
    mlog(ALW, "thread_c7 #2");

    c7::thread::thread th3;
    mlog(ALW, "thread_c7 #3");
}


static void thread_std()
{
    mlog(ALW, "thread_std #1");

    c7::thread::thread th2;
    mlog(ALW, "thread_std #2");

    c7::thread::thread th3;
    mlog(ALW, "thread_std #3");
}


int main(int argc, char **argv)
{
    mlog(ALW, "main #1");

    c7::thread::thread th2;
    mlog(ALW, "main #2");

    c7::thread::thread th;
    th.target(thread_c7);
    th.start();
    th.join();

    std::thread sth{thread_std};
    sth.join();

    c7::thread::thread th3;
    mlog(ALW, "main #3");

    return 0;
}
