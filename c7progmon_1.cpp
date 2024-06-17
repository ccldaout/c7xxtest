#include <sys/wait.h>
#include <c7app.hpp>
#include <c7progmon.hpp>


using c7::p_;
using c7::P_;


struct my_data {
    uint64_t counter;
};


class my_datamon: public c7::progress_monitor<my_data> {
protected:
    void check(uint64_t not_updated_count, pid_t parent_pid, my_data& data) override {
	p_("--- #not_updated:%{}, parent:%{}, counter:%{} ---",
	   not_updated_count, parent_pid, data.counter);
    }
};


class my_datamon2: public c7::progress_monitor<my_data> {
protected:
    void check(uint64_t not_updated_count, pid_t parent_pid, my_data& data) override {
	p_("--- #not_updated:%{}, parent:%{}, counter:%{} ---",
	   not_updated_count, parent_pid, data.counter);
	if (not_updated_count == 4) {
	    p_("\n--- kill parent ---\n");
	    ::kill(parent_pid, SIGKILL);
	}
    }
};


class my_voidmon: public c7::progress_monitor<void> {
protected:
    void check(uint64_t not_updated_count, pid_t parent_pid) override {
	p_("*** #not_updated:%{}, parent:%{} ***",
	   not_updated_count, parent_pid);
    }
};


int main()
{
    pid_t pid;

    p_("----------------- data monitor ----------------\n");

    pid = ::fork();
    if (pid == 0) {
	my_datamon datamon;
	datamon.start_monitor(100);

	for (int i = 0; i < 200; i++) {
	    c7::sleep_ms(10);
	    datamon.update().counter++;
	}
	
	c7::sleep_ms(800);
	p_("--- stop_monitor ---");
	datamon.stop_monitor();
	c7::sleep_ms(200);
	p_("--- exit ---");
	std::quick_exit(EXIT_SUCCESS);
    }
    waitpid(pid, nullptr, 0);

    p_("\n----------------- data monitor2 ----------------\n");

    pid = ::fork();
    if (pid == 0) {
	my_datamon2 datamon;
	datamon.start_monitor(100);

	for (int i = 0; i < 200; i++) {
	    c7::sleep_ms(10);
	    datamon.update().counter++;
	}
	
	c7::sleep_ms(1000);
	p_("--- exit ---");
	std::quick_exit(EXIT_SUCCESS);
    }
    waitpid(pid, nullptr, 0);

    p_("\n----------------- void monitor ----------------\n");

    my_voidmon voidmon;
    voidmon.start_monitor(100);
    for (int i = 0; i < 200; i++) {
	c7::sleep_ms(10);
	voidmon.update();
    }

    c7::sleep_ms(1000);
    p_("main exit");
}
