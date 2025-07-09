#include <fcntl.h>
#include <c7app.hpp>
#include <c7mm.hpp>
#include <c7rawbuf.hpp>
#include <c7seq.hpp>

using c7::p_;

struct data_t {
    data_t() {
	s[0] = 0;
    }
    explicit data_t(const char *str) {
	std::strcpy(s, str);
    }

    char s[100];
};

c7::fd test_1()
{
    p_("-- [test_1] --");

    c7::rawbuf<data_t> rb;

    if (auto res = rb.reserve(100); !res) {
	c7error(res, "reserve");
    }

    data_t d1{"abc"};
    if (auto res = rb.push_back(d1); !res) {
	c7error(res, "push_back: %{}", d1.s);
    }

    const char *args[] = {
	"hello", "world", "c++", "language"
    };
    for (auto a: args) {
	if (auto res = rb.emplace_back(a); !res) {
	    c7error(res, "push_back: %{}", a);
	}
    }

    p_("... rb ...");
    for (auto& d: rb) {
	c7echo("&d:%{}, d.s:<%{}>", &d, d.s);
    }

    c7::fd fd;
    if (auto res = c7::open("../TEST.rabuf", O_CREAT|O_RDWR|O_TRUNC); !res) {
	c7error(std::move(res));
    } else {
	fd = std::move(res.value());
    }
    if (auto res = rb.flush_to(fd); !res) {
	c7error(res, "flush_to");
    }

    p_("... rb after rb.flush_to(fd) ...");
    for (auto& d: rb) {
	c7echo("&d:%{}, d.s:<%{}>", &d, d.s);
    }

    return fd;
}

void test_2(c7::fd& fd)
{
    p_("-- [test_2] --");

    c7::rawbuf<data_t, c7::anon_mmap_manager> rb;
    data_t d1{"test-2 beg"};
    if (auto res = rb.push_back(d1); !res) {
	c7error(res, "push_back: %{}", d1.s);
    }

    if (auto res = fd.seek_abs(0); !res) {
	c7error(res, "fd.seek_abs");
    }
    if (auto res = rb.append_from(fd); !res) {
	c7error(res, "append_from");
    }

    data_t d2{"test-2 end"};
    if (auto res = rb.push_back(d2); !res) {
	c7error(res, "push_back: %{}", d1.s);
    }

    p_("... rb reversed ...");
    for (auto& d: c7::seq::reverse(rb)) {
	c7echo("&d:%{}, d.s:<%{}>", &d, d.s);
    }
}

void test_3(c7::fd& fd)
{
    p_("-- [test_3] --");

    c7::rawbuf<data_t, c7::anon_mmap_manager> rb;
    data_t d1{"test-3 beg"};
    if (auto res = rb.push_back(d1); !res) {
	c7error(res, "push_back: %{}", d1.s);
    }
    data_t d2{"test-3 end"};
    if (auto res = rb.push_back(d2); !res) {
	c7error(res, "push_back: %{}", d1.s);
    }

    if (auto res = fd.seek_abs(sizeof(d1)); !res) {
	c7error(res, "fd.seek_abs(%{})", sizeof(d1));
    }
    if (auto res = rb.insert_from(fd, 1); !res) {
	c7error(res, "insert_from(fd, 1)");
    }

    p_("... rb after insert_from(fd, 1) ...");
    for (auto& d: rb) {
	c7echo("&d:%{}, d.s:<%{}>", &d, d.s);
    }

    rb.resize(3);
    p_("... rb after resize(3) ...");
    for (auto& d: rb) {
	c7echo("&d:%{}, d.s:<%{}>", &d, d.s);
    }

    rb.resize(5);
    p_("... rb after resize(5) ...");
    for (auto& d: rb) {
	c7echo("&d:%{}, d.s:<%{}>", &d, d.s);
    }
}

int main()
{
    auto fd = test_1();
    test_2(fd);
    test_3(fd);
    return 0;
}
