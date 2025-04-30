#include <c7format.hpp>
#include <c7signal.hpp>
#include <c7utils/time.hpp>


using c7::p_;


int main(int argc, char **argv)
{
    c7::signal::sig_handler sighandle = [argv](int sig) {
					    p_("** %{}::sighandle %{}", argv[0], sig);
					};

    p_("-- SIGINT SIG_IGN");
    c7::signal::signal(SIGINT, SIG_IGN);
    c7::sleep_ms(1000);

    p_("   raise SIGINT & sleep_ms(300)");
    ::kill(getpid(), SIGINT);
    c7::sleep_ms(300);

    p_("-- SIGINT sig_handler");
    auto old_handler = c7::signal::signal(SIGINT, sighandle);
    c7::sleep_ms(100);

    p_("   raise SIGINT & sleep_ms(300)");
    ::kill(getpid(), SIGINT);
    c7::sleep_ms(300);

    p_("-- SIGINT old_handler");
    c7::signal::signal(SIGINT, old_handler);
    c7::sleep_ms(100);

    p_("   raise SIGINT & sleep_ms(300)");
    ::kill(getpid(), SIGINT);
    c7::sleep_ms(300);

    p_("-- SIGINT SIG_DFL");
    c7::signal::signal(SIGINT, SIG_DFL);
    c7::sleep_ms(100);

    p_("   raise SIGINT & sleep_ms(300)");
    ::kill(getpid(), SIGINT);
    c7::sleep_ms(300);

    p_("main end");
}
