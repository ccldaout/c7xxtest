#include <c7app.hpp>
#include <c7thread/thread.hpp>
#include <c7utils.hpp>
#include <c7event/submit.hpp>


struct test {
    std::shared_ptr<c7::event::submit_provider> spp;
    c7::thread::thread th;

    void thread() {
	c7echo("thread: id:%{}", c7::thread::self::id());
	c7::sleep_ms(500);
	c7echo("thread: submit ...");
	c7::sleep_ms(500);
	spp->submit([this](){ callback(); });
    }

    void callback() {
	c7echo("callback: id:%{}", c7::thread::self::id());
	th.join();
	c7echo("joined !!");
	std::exit(0);
    }
};

int main()
{
    test t;
    if (auto res = c7::event::submit_provider::make_and_manage(); !res) {
	c7abort(res);
    } else {
	t.spp = std::move(res.value());
    }

    t.th.target([&t](){ t.thread(); });
    t.th.start();

    c7echo("loop start: id:%{}", c7::thread::self::id());
    c7::event::forever();
}

