#include <c7format.hpp>
#include <c7signal.hpp>
#include <c7threadsync.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;

using c7::p_;

static c7::thread::event do_exit;

static void (*restore)(int);

static void sigint(int sig)
{
    static int rest = 5;
    p_("sig:%{}, rest:%{}", sig, rest);
    if (rest-- == 0) {
	c7::signal::handle(SIGINT, restore);
	//do_exit.set();
    }
}

int main()
{
    std::string s;

    p_("signal setup ...");
    if (auto res = c7::signal::handle(SIGINT, sigint); res) {
	restore = res.value();
    }

    p_("SIGINT blocked");
    auto unblock = c7::signal::block(SIGINT);

    p_("enter anything (NOT EMPTY STRING) ...");
    cin >> s;

    unblock();
    p_("SIGINT unblocked");

    do_exit.wait();

    return 0;
}
