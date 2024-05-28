#include <unistd.h>
#include <sys/stat.h>
#include <c7app.hpp>
#include <c7file.hpp>
#include <c7pipeline.hpp>
#include <c7socket.hpp>
#include <c7thread/thread.hpp>


using c7::p_;
using c7::P_;


static void show_usedfd(int maxfd)
{
    struct stat stbuf;
    P_("used fd: ");
    for (int i = 0; i <= maxfd; i++) {
	if (::fstat(i, &stbuf) == C7_SYSOK) {
	    P_(" %{}", i);
	}
    }
    p_("");
}


static void test_1()
{
    p_("[test 1] -----------------------------------------");

    show_usedfd(20);

    c7::pipeline pl;

    auto res = pl
	.add(".", "", {"ls", "-l"}, {"LC_ALL=C"})
	.add(".", "", {"head", "-n5"})
	.add(".", "", {"tr", "a-z", "A-Z"})
	.exec(::dup(0), ::dup(1), 2);

    if (!res) {
	c7error(res);
    }

    auto sv = pl.wait();
    for (auto s: sv) {
	p_("  status: %{}", s);
    }

    show_usedfd(20);
}


static void test_2()
{
    p_("[test 2] -----------------------------------------");

    show_usedfd(20);

    c7::pipeline pl;

    auto res = pl
	.add(".", "", {"head", "-n20"})
	.add(".", "", {"tr", "a-z", "A-Z"})
	.exec();

    if (!res) {
	c7error(res);
    }
    c7::socket sock{res.value()};

    c7::thread::thread th;
    th.target([&sock](){
	    auto ls = std::move(c7::file::readlines("c7pipeline_1.cpp").value());
	    for (auto& s: ls) {
		sock.write_n(s.c_str(), s.size());
		sock.write_n("\n", 1);
	    }
	    sock.shutdown_w();
	});
    th.start();

    for (;;) {
	char buf[512];
	if (auto res = sock.read(buf); !res) {
	    p_("read: %{}", res);
	    break;
	} else {
	    c7::drop = write(1, buf, res.value());
	    if (res.value() == 0) {
		break;
	    }
	}
    }
    sock.close();

    auto sv = pl.wait();
    for (auto s: sv) {
	p_("  status: %{}", s);
    }

    show_usedfd(20);
}


static void test_3()
{
    p_("[test 3] -----------------------------------------");

    show_usedfd(20);

    c7::pipeline pl;
    int fd[2];

    auto res = pl
	.add(".", "", {"tail", "-n15"})
	.add(".", "", {"tr", "-s", " \t", "_"})
	.exec(fd);

    if (!res) {
	c7error(res);
    }

    c7::thread::thread th;
    th.target([&fd](){
	    c7::fd wfd{fd[1]};
	    auto ls = std::move(c7::file::readlines("c7pipeline_1.cpp").value());
	    for (auto& s: ls) {
		wfd.write_n(s.c_str(), s.size());
		wfd.write_n("\n", 1);
	    }
	});
    th.start();

    c7::fd rfd{fd[0]};
    for (;;) {
	char buf[512];
	if (auto res = rfd.read(buf); !res) {
	    p_("read: %{}", res);
	    break;
	} else {
	    c7::drop = write(1, buf, res.value());
	    if (res.value() == 0) {
		break;
	    }
	}
    }
    rfd.close();

    auto sv = pl.wait();
    for (auto s: sv) {
	p_("  status: %{}", s);
    }

    show_usedfd(20);
}


static void test_4()
{
    p_("[test 4] -----------------------------------------");

    show_usedfd(20);

    c7::pipeline pl;
    int fd[2];

    auto res = pl
	.add(".", "", {"tr", "a-z", "A-Z"})
	.exec(fd);

    if (!res) {
	c7error(res);
    }

    std::string s{"Hello world!\n"};
    c7::fd wfd{fd[1]};
    wfd.write_n(s.c_str(), s.size());
    wfd.close();

    c7::fd rfd{fd[0]};

    char buf[512];
    if (auto res = rfd.read(buf, sizeof(buf)); !res) {
	p_("read: %{}", res);
    } else {
	c7::drop = write(1, buf, res.value());
    }
    rfd.close();

    auto sv = pl.wait();
    for (auto s: sv) {
	p_("  status: %{}", s);
    }

    show_usedfd(20);
}


int main()
{
    test_1();
    test_2();
    test_3();
    test_4();
}
