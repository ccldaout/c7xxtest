#include <c7thread/queue.hpp>
#include <c7thread/thread.hpp>
#include <c7app.hpp>
#include <list>


using namespace c7::thread;


using str_uptr = std::unique_ptr<std::string>;


static void consumer(queue<str_uptr>& que)
{
    c7::usec_t tmo_us = 200 * 1000;
    for (;;) {
	auto res = que.get(tmo_us);
	if (res) {
	    auto sup = std::move(res.value());
	    c7echo("consumer: que.get: str:%{}, size:%{}", *sup, sup->size());
	} else {
	    c7echo("consumer: que.get: is_readable:%{}, is_closed:%{}, is_aborted:%{}\n%{}",
		   que.is_alive(), que.is_closed(), que.is_aborted(), res);
	    if (not que.is_alive()) {
		c7echo("consumer: return (no readable)");
		return;
	    }
	}
    }
}

static void test_queue()
{
    queue<str_uptr> que;

    thread th;
    th.target([&que](){ consumer(que); });
    th.start();

    if (auto res = que.put(str_uptr(new std::string("hello"))); !res) {
	c7error("producer: que.put: %{}", res);
    }
    if (auto res = que.put(str_uptr(new std::string("world"))); !res) {
	c7error("producer: que.put: %{}", res);
    }
    c7::sleep_us(300*1000);
    if (auto res = que.put(str_uptr(new std::string("finish"))); !res) {
	c7error("producer: que.put: %{}", res);
    }
    c7echo("producer: que.close");
    que.close();
    c7echo("producer: sleep ...");
    c7::sleep_us(100*1000);

    th.join();
}


static void consumer(sync_queue<str_uptr>& sq)
{
    c7::usec_t tmo_us = 200 * 1000;
    for (;;) {
	auto res = sq.get(tmo_us);
	if (res) {
	    auto sup = std::move(res.value());
	    c7echo("consumer: sq.get: str:%{}, size:%{}", *sup, sup->size());
	} else {
	    c7echo("consumer: sq.get: is_alive:%{}, is_closed:%{}, is_aborted:%{}\n%{}",
		   sq.is_alive(), sq.is_closed(), sq.is_aborted(), res);
	    if (not sq.is_alive()) {
		c7echo("consumer: sq.sync_finish (not readable)");
		auto res = sq.sync_finish();
		c7echo("consumer: sq.sync_finish: %{}", res);
		c7echo("consumer: return (not readable)");
		return;
	    }
	}
    }
}

static void test_sync_queue()
{
    sync_queue<str_uptr> sq;

    thread th;
    th.target([&sq](){ consumer(sq); });
    th.start();

    if (auto res = sq.put(str_uptr(new std::string("hello"))); !res) {
	c7error("producer: sq.put: %{}", res);
    }
    if (auto res = sq.put(str_uptr(new std::string("world"))); !res) {
	c7error("producer: sq.put: %{}", res);
    }
    c7::sleep_us(300*1000);
    if (auto res = sq.put(str_uptr(new std::string("finish"))); !res) {
	c7error("producer: sq.put: %{}", res);
    }
    c7echo("producer: sq.close");
    sq.close();
    c7echo("producer: sleep ...");
    c7::sleep_us(100*1000);
    c7echo("producer: sq.sync_finish");
    auto res = sq.sync_finish();
    c7echo("producer: sq.sync_finish: %{}", res);

    th.join();
}


static void test_scan_queue()
{
    queue<str_uptr> que;

    if (auto res = que.put(str_uptr(new std::string("hello"))); !res) {
	c7error("producer: que.put: %{}", res);
    }
    if (auto res = que.put(str_uptr(new std::string("world"))); !res) {
	c7error("producer: que.put: %{}", res);
    }
    auto res = que.scan([](auto *uptr_p){
	    c7echo("que.scan: <%{}>", **uptr_p);
	    return c7result_ok();
	});
}


int main()
{
    test_queue();
    c7::p_("");
    test_sync_queue();
    c7::p_("");
    test_scan_queue();
}
