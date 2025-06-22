#include <c7app.hpp>
#include <c7dconf.hpp>
#include <c7thread/thread.hpp>
#include <c7utils.hpp>
#include <c7event/submit.hpp>


struct test {
    c7::thread::thread th;

    void thread() {
	c7echo("thread: id:%{}", c7::thread::self::id());
	c7::sleep_ms(500);
	c7echo("thread: submit ...");
	c7::event::submit([this](){ callback(); });
	c7echo("thread: submit ... end and wait 500ms");
	c7::sleep_ms(500);
	c7echo("end thread");
    }

    void callback() {
	c7echo("callback: id:%{}, submit(self)", c7::thread::self::id());
	c7::event::submit([this](){ callback(); });
	c7echo("callback: id:%{}, join ...", c7::thread::self::id());
	th.join();
	c7echo("joined !!");
	std::exit(0);
    }
};

int main()
{
    c7::dconf[C7_DCONF_MLOG_LIBC7].i = 7;

    test t;

    t.th.target([&t](){ t.thread(); });
    t.th.start();

    c7echo("loop start: id:%{}", c7::thread::self::id());
    c7::event::forever();
}

