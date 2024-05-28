#include <c7thread/jobque.hpp>
#include <c7thread/thread.hpp>
#include <c7app.hpp>
#include <c7file.hpp>
#include <list>


using namespace c7::thread;


using str_uptr = std::unique_ptr<std::string>;


static void consumer(jobque<str_uptr>& jq)
{
    c7::usec_t tmo_us = 200 * 1000;
    for (;;) {
	auto res = jq.get(tmo_us);
	if (res) {
	    auto sup = std::move(res.value());
	    c7echo("consumer: jq.get: str:%{}, size:%{}", *sup, sup->size());
	    jq.commit();	// IMPORTANT for jobque
	} else {
	    c7echo("consumer: jq.get: is_alive:%{}, is_closed:%{}, is_aborted:%{}\n%{}",
		   jq.is_alive(), jq.is_closed(), jq.is_aborted(), res);
	    if (not jq.is_alive()) {
		c7echo("consumer: return (not is_alive)");
		return;
	    }
	}
    }

}

static void test_jobque()
{
    jobque<str_uptr> jq;

    thread th;
    th.target([&jq](){ consumer(jq); });
    th.finalize([&jq](){ jq.abort(); });
    th.start();

    if (auto res = jq.put(str_uptr(new std::string("hello"))); !res) {
	c7error("produccer: jq.put: %{}", res);
    }
    if (auto res = jq.put(str_uptr(new std::string("world"))); !res) {
	c7error("produccer: jq.put: %{}", res);
    }
    c7::sleep_us(300*1000);
    if (auto res = jq.put(str_uptr(new std::string("finish"))); !res) {
	c7echo("consumer thread: %{}", th.terminate_result());
	c7error("produccer: jq.put: %{}", res);
    }
    c7echo("produccer: jq.close");
    jq.close();
    c7echo("produccer: jq.wait_finished ...");
    auto res = jq.wait_finished();
    c7echo("produccer: jq.wait_finished: %{}", res);

    th.join();
    c7echo("consumer thread: %{}", th.terminate_result());
}


static void consumer(weight_jobque<str_uptr>& wq)
{
    c7::usec_t tmo_us = 200 * 1000;
    size_t weight;
    c7::sleep_us(200*1000);
    for (;;) {
	auto res = wq.get(weight, tmo_us);
	if (res) {
	    auto sup = std::move(res.value());
	    c7echo("consumer: wq.get: size:%{3} str:%{}", sup->size(), *sup);
	    c7::sleep_us(50*1000);
	    wq.commit(weight);	// IMPORTANT for jobque
	} else {
	    c7echo("consumer: wq.get: is_alive:%{}, is_closed:%{}, is_aborted:%{}\n%{}",
		   wq.is_alive(), wq.is_closed(), wq.is_aborted(), res);
	    if (not wq.is_alive()) {
		c7echo("consumer: return (not is_alive)");
		return;
	    }
	}
    }

}

static void test_weight_jobque(const char *path)
{
    weight_jobque<str_uptr> wq;
    wq.set_limit(200);

    thread th;
    th.target([&wq](){ consumer(wq); });
    th.start();

    if (auto res = c7::file::readlines(path); !res) {
	c7error(res);
    } else {
	for (auto& s: res.value()) {
	    auto sup = str_uptr(new std::string(s));
	    if (auto res = wq.put(std::move(sup), s.size(), 10); !res) {
		if (wq.is_alive()) {
		    wq.wait_progress(100)
			>> ([](auto&& v){
				c7echo("producer: wait_progress -> %{}", v);
				return c7result_ok();
			    });
		} else {
		    c7error(res, "producer: wq.put");
		}
	    } else {
		c7echo("producer: wq.put: size:%{3} str:%{}", s.size(), s);
	    }
	}
    }

    c7echo("produccer: wq.close");
    wq.close();
    c7echo("produccer: wq.wait_finished ...");
    auto res = wq.wait_finished();
    c7echo("produccer: wq.wait_finished: %{}", res);
    th.join();
}


static void test_scan_weight_jobque(const char *path)
{
    weight_jobque<str_uptr> wq;
    wq.set_limit(100);

    if (auto res = c7::file::readlines(path); !res) {
	c7error(res);
    } else {
	auto tmo_us = 1000;
	for (auto& s: res.value()) {
	    auto sup = str_uptr(new std::string(s));
	    if (auto res = wq.put(std::move(sup), s.size(), tmo_us); !res) {
		c7echo(res, "producer: wq.put: %{}", res);
		wq.scan([](auto *item) {
			auto uptr_p = &item->first;
			c7echo("scan: %{3} : %{}", item->second, **uptr_p);
			return c7result_ok();
		    });
		return;
	    } else {
		c7echo("producer: wq.put: size:%{3} str:%{}", s.size(), s);
	    }
	}
    }
}

int main(int argc, char **argv)
{
    if (argc != 2) {
	c7error("Usage: %{} file", c7::app::progname);
    }
    test_jobque();
    c7::p_("\n -------------------------------------------- ");
    test_weight_jobque(argv[1]);
    c7::p_("\n -------------------------------------------- ");
    test_scan_weight_jobque(argv[1]);
}
