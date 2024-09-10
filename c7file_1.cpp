#include <c7format.hpp>
#include <c7file.hpp>
#include <c7path.hpp>

#include <unistd.h>

using namespace std;

using c7::p_;

struct data_t {
    char name[32];
    int age;
    int code;
};


static void mkdir_test()
{
    p_("--- mkdir --------------------------------");
    {
	auto dir = "~/tmp/abc/def";
	auto res = c7::file::mkdir(c7::path::untildize(dir));
	if (res)
	    p_("ok: %{}", dir);
	else
	    p_("ng: %{}", res);
    }
    {
	auto dir = c7::path::untildize("~/tmp/abc/def");
	auto res = c7::file::mkdir(dir);
	if (res)
	    p_("ok: %{}", dir);
	else
	    p_("ng: %{}", res);
    }
    {
	auto dir = c7::path::untildize("~/tmp/abc/def");
	::rmdir(dir.c_str());
	auto res = c7::file::mkdir(dir, 0700, 1, 1);
	if (res)
	    p_("ok: %{}", dir);
	else
	    p_("ng: %{}", res);
    }
    {
	auto dir = c7::path::untildize("/proc/tmp/abc/def");
	auto res = c7::file::mkdir(dir);
	if (res)
	    p_("ok: %{}", dir);
	else
	    p_("ng: %{}", res);
    }
}


static void rw_test()
{
    auto path = c7::path::untildize("~/tmp/test.rw");

    p_("--- rw --------------------------------");
    {
	data_t data[3] = {
	    { .age = 30, .code = 0x30 },
	    { .age = 20, .code = 0x20 },
	    { .age = 10, .code = 0x10 },
	};
	(void)strcpy(data[0].name, "C");
	(void)strcpy(data[1].name, "C++");
	(void)strcpy(data[2].name, "Rust");
	auto res = c7::file::write(path, 0600, data, sizeof(data));
	p_("write#1 %{}", res);
	if (res) {
	    struct buff_t {
		data_t data;
		char dummy;
	    };
	    buff_t buf;
	    auto res = c7::file::read_into(path, &buf, sizeof(buf));
	    if (!res)
		p_("read#1 %{}", res);
	    else {
		p_("read#1 value:%{}", res.value());
		auto d = &buf.data;
		p_("name:<%{}>, age:%{}, code:%{#x}", d->name, d->age, d->code);
	    }
	}
    }

    p_("-- read<data_t[]> --");
    {
	size_t sz = 0;
	auto res = c7::file::read<data_t[]>(path, sz);
	if (!res)
	    p_("r ng: %{}", res);
	else {
	    auto d = std::move(res.value());
	    for (decltype(sz) n = 0; n < sz/sizeof(d[0]); n++) {
		p_("[%{}] name:<%{}>, age:%{}, code:%{#x}", n, d[n].name, d[n].age, d[n].code);
	    }
	}
    }

    p_("-- rewrite --");
    {
	data_t data{ .age = 15, .code = 0x15 };
	(void)strcpy(data.name, "python");
	auto res = c7::file::rewrite(path, &data, sizeof(data));
	p_("rewrite#1 %{}", res);
	if (res) {
	    data_t data;
	    auto res = c7::file::read_into(path, &data, sizeof(data));
	    if (!res)
		p_("read#2 %{}", res);
	    else {
		p_("read#2 value:%{}", res.value());
		auto d = &data;
		p_("name:<%{}>, age:%{}, code:%{#x}", d->name, d->age, d->code);
	    }
	}
    }

    p_("-- read --");
    {
	auto res = c7::file::read<data_t>(path);
	if (!res)
	    p_("read(entire)#1 %{}", res);
	else {
	    auto d = res.value().get();
	    p_("name:<%{}>, age:%{}, code:%{#x}", d->name, d->age, d->code);
	}
    }

#if 1
    p_("-- read stdin -- SKIPPED --");
#else
    p_("-- read stdin --");
    {
	auto res = c7::file::read<char>("");		// read from stdin (fd 0)
	if (!res)
	    p_("read(entire)#2 %{}", res);
	else {
	    p_("%{}", res.value().get());
	}
    }
#endif

    p_("-- readlines --");
    {
	auto res = c7::file::readlines(c7::path::untildize("~/.gitconfig"));
	if (!res)
	    p_("readlines: %{}");
	else {
	    for (auto& s: res.value()) {
		p_("<%{}>", s);
	    }
	}
    }
}


static void mmap_test()
{
    p_("--- mmap --------------------------------");

    size_t size = sizeof(data_t);
    auto path = c7::path::untildize("~/tmp/test.mmap");

    if (::unlink(path.c_str()) == C7_SYSERR)
	p_("unlink failed: errno:%{}", errno);

    {
	auto res = c7::file::mmap_rw<data_t>(path, size, true);
	if (!res)
	    p_("rw ng: %{}", res);
	else {
	    auto& d = res.value();
	    (void)std::strcpy(d->name, "miwanaga");
	    d->age = 53;
	    d->code = 0x1234;
	}
    }

    {
	auto res = c7::file::mmap_r<data_t>(path);
	if (!res)
	    p_("r ng: %{}", res);
	else {
	    auto d = std::move(res.value());
	    p_("name:<%{}>, age:%{}, code:%{#x}", d->name, d->age, d->code);
	    p_("after: res.value: %{p}", res.value().get());
	}
    }

    {
	auto res = c7::file::mmap_rw(path);
	if (!res)
	    p_("rw#2 ng: %{}", res);
	else {
	    auto d = static_cast<data_t*>(res.value().get());
	    d->name[1]= 'I';
	    d->age++;
	    d->code = 0xabcd;
	}
    }

    {
	auto res = c7::file::mmap_r(path);
	if (!res)
	    p_("r ng: %{}", res);
	else {
	    auto d = static_cast<data_t*>(res.value().get());
	    p_("name:<%{}>, age:%{}, code:%{#x}", d->name, d->age, d->code);
	    p_("after: res.value: %{p}", res.value().get());
	}
    }

    p_("mmap_rw<data_t[]>");
    {
	size_t sz = sizeof(data_t) * 3;
	auto res = c7::file::mmap_rw<data_t[]>(path, sz, true);
	if (!res)
	    p_("rw ng: %{}", res);
	else {
	    auto& d = res.value();
	    (void)std::strcpy(d[0].name, "C");
	    d[0].age = 30;
	    d[0].code = 0x30;
	    (void)std::strcpy(d[1].name, "C++");
	    d[1].age = 20;
	    d[1].code = 0x20;
	    (void)std::strcpy(d[2].name, "Rust");
	    d[2].age = 10;
	    d[2].code = 0x10;
	}
    }
    
    p_("mmap_r<data_t[]>");
    {
	size_t sz = 0;
	auto res = c7::file::mmap_r<data_t[]>(path, sz);
	if (!res)
	    p_("r ng: %{}", res);
	else {
	    auto d = std::move(res.value());
	    for (decltype(sz) n = 0; n < sz/sizeof(d[0]); n++) {
		p_("[%{}] name:<%{}>, age:%{}, code:%{#x}", n, d[n].name, d[n].age, d[n].code);
	    }
	}
    }
}


int main()
{
    mkdir_test();
    mmap_test();
    rw_test();

    return 0;
}
