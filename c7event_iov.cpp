#define C7_IOVEC_PROXY_DEBUG 1

#include <c7app.hpp>
#include <c7event/iovec_proxy.hpp>
#include <c7slice.hpp>

using namespace c7::event;

using c7::p_;
using c7::P_;

static void check(const iovec_proxy& iov,
		  const void *expect_base,
		  size_t expect_len)
{
    if (iov.iov_base != expect_base ||
	iov.iov_len  != expect_len) {
	p_("*ERR*\tiov_base:%{}\n"
	   "\t  expect:%{}\n"
	   "\t iov_len:%{}\n"
	   "\t  expect:%{}",
	   iov.iov_base, expect_base,
	   iov.iov_len,  expect_len);
    }
}

static void assign_test()
{
    p_("ASSIGN_TEST ----------------------------------------");

    ::iovec base;
    iovec_proxy iov{base};

    // -- basic

    {
	p_("\n-- const iov_base_t& + iov_len");
	::iovec cbase{nullptr, 123};
	const iovec_proxy ciov{cbase};
	iov.iov_base = ciov.iov_base;
	iov.iov_len  = ciov.iov_len;
	check(iov, nullptr, 123);
    }

    {
	p_("\n-- nullptr");
	iov.iov_base = nullptr;
	check(iov, nullptr, 0);
    }

    {
	p_("\n-- int *");
	int v = 1;
	iov.iov_base = &v;
	check(iov, &v, sizeof(v));
    }
    {
	p_("\n-- const int *");
	const int v = 1;
	iov.iov_base = &v;
	check(iov, &v, sizeof(v));
    }

    // -- struct

    {
	p_("\n-- struct *");
	struct { int a; double b; } v{};
	iov.iov_base = &v;
	check(iov, &v, sizeof(v));
    }
    {
	p_("\n-- const struct *");
	const struct { int a; double b; } v{};
	iov.iov_base = &v;
	check(iov, &v, sizeof(v));
    }

    // -- char*

    {
	p_("\n-- char * (3+NUL)");
	char s[] = "abc";
	char *v = s;
	iov.iov_base = v;
	check(iov, v, strlen(v)+1);
    }

    {
	p_("\n-- const char * (3+NUL)");
	const char *v = "abc";
	iov.iov_base = v;
	check(iov, v, strlen(v)+1);
    }

    // -- string

    {
	p_("\n-- std::string (3)");
	std::string v{"abc"};
	iov.iov_base = v;
	check(iov, v.c_str(), v.size()+1);
    }
    {
	p_("\n-- const std::string (3)");
	const std::string v{"abc"};
	iov.iov_base = v;
	check(iov, v.c_str(), v.size()+1);
    }
    {
	p_("\n-- std::string * (3)");
	std::string v{"abc"};
	iov.iov_base = &v;
	check(iov, v.c_str(), v.size()+1);
    }
    {
	p_("\n-- const std::string *");
	const std::string v{"abc"};
	iov.iov_base = &v;
	check(iov, v.c_str(), v.size()+1);
    }

    // -- string_view

    {
	p_("\n-- std::string_view& (3)");
	std::string vv{" abc "};
	std::string_view v{vv.c_str()+1, 3};
	iov.iov_base = v;
	check(iov, vv.c_str()+1, 3+1);
    }
    {
	p_("\n-- const std::string_view&");
	std::string vv{" abc "};
	const std::string_view v{vv.c_str()+1, 3};
	iov.iov_base = v;
	check(iov, vv.c_str()+1, 3+1);
    }
    {
	p_("\n-- std::string_view * (3)");
	std::string vv{" abc "};
	std::string_view v{vv.c_str()+1, 3};
	iov.iov_base = &v;
	check(iov, vv.c_str()+1, 3+1);
    }
    {
	p_("\n-- const std::string_view *");
	std::string vv{" abc "};
	const std::string_view v{vv.c_str()+1, 3};
	iov.iov_base = &v;
	check(iov, vv.c_str()+1, 3+1);
    }

    // -- vector

    {
	p_("\n-- std::vector<int> (3)");
	std::vector<int> v{1,2,3};
	iov.iov_base = v;
	check(iov, v.data(), v.size()*sizeof(int));
    }
    {
	p_("\n-- const std::vector<int> (3)");
	const std::vector<int> v{1,2,3};
	iov.iov_base = v;
	check(iov, v.data(), v.size()*sizeof(int));
    }
    {
	p_("\n-- std::vector<int> * (3)");
	std::vector<int> v{1,2,3};
	iov.iov_base = &v;
	check(iov, v.data(), v.size()*sizeof(int));
    }
    {
	p_("\n-- const std::vector<int> * (3)");
	const std::vector<int> v{1,2,3};
	iov.iov_base = &v;
	check(iov, v.data(), v.size()*sizeof(int));
    }

    // -- array

    {
	p_("\n-- std::array<int,3>");
	std::array<int, 3> v{1,2,3};
	iov.iov_base = v;
	check(iov, v.data(), v.size()*sizeof(int));
    }
    {
	p_("\n-- const std::array<int,3>");
	const std::array<int, 3> v{1,2,3};
	iov.iov_base = v;
	check(iov, v.data(), v.size()*sizeof(int));
    }
    {
	p_("\n-- std::array<int,3>*");
	std::array<int, 3> v{1,2,3};
	iov.iov_base = &v;
	check(iov, v.data(), v.size()*sizeof(int));
    }
    {
	p_("\n-- const std::array<int,3>*");
	const std::array<int, 3> v{1,2,3};
	iov.iov_base = &v;
	check(iov, v.data(), v.size()*sizeof(int));
    }

    // -- slice

    {
	p_("\n-- slice<std::vector<int>,off:1,gap:1,n:3>*");
	std::vector<int> vv{1,2,3,4,5};
	c7::slice v = c7::make_slice(vv, 1, 1, 3);
	iov.iov_base = &v;
	check(iov, &vv[1], 3*sizeof(int));
    }

    // -- void*

    {
	p_("\n-- void *");
	int vv = 0;
	void *v = &vv;
	iov.iov_base = v;
	auto olen = iov.iov_len;
	check(iov, &vv, olen);
    }
    {
	p_("\n-- const void *");
	int vv = 0;
	const void *v = &vv;
	iov.iov_base = v;
	auto olen = iov.iov_len;
	check(iov, &vv, olen);
    }
    {
	p_("\n-- void **");
	int vv = 0;
	void *v = &vv;
	iov.iov_base = &v;
	auto olen = iov.iov_len;
	check(iov, &v, olen);
    }

    // -- as<T>
    {
	const iovec_proxy& ciov{iov};
	[[maybe_unused]] int *p = iov.iov_base.as<int>();
	[[maybe_unused]] const int *cp = iov.iov_base.as<int>();
	cp = iov.iov_base.as<const int>();

	[[maybe_unused]] long *lp = ciov.iov_base.as<long>();
	[[maybe_unused]] const long *clp = ciov.iov_base.as<long>();
	clp = ciov.iov_base.as<const long>();
    }

    // -- operator T
    {
	const iovec_proxy& ciov{iov};
	[[maybe_unused]] int *p = iov.iov_base;
	[[maybe_unused]] const int *cp = iov.iov_base;

	[[maybe_unused]] long *lp = ciov.iov_base;
	[[maybe_unused]] const long *clp = ciov.iov_base;
    }
}

template <typename C>
static void show(const char *pref, const C& c)
{
    P_("%{}:", pref);
    for (const auto& v: c) {
	P_(" %{}", v);
    }
    p_("");
}

static void get_test()
{
    ::iovec base;
    iovec_proxy iov{base};

    p_("GET_TEST ----------------------------------------");

    {
	p_("\n-- slice");
	std::vector<int> vv{1,2,3,4,5};
	c7::slice<int> v;
	iov.iov_base = vv;
	p_("    slice(v)");
	if (auto res = iov.strict_slice(v); !res) {
	    c7echo("\t%{}", res);
	} else {
	    check(iov, v.data(), v.size()*sizeof(int));
	}
	p_("    slice(v, 50) -> error");
	if (auto res = iov.strict_slice(v, 50); !res) {
	    c7echo("%{}", res);
	}

	const iovec_proxy ciov{iov};
	if (auto res = ciov.strict_slice(v); !res) {
	    c7echo("\t%{}", res);
	} else {
	    v[1] *= 100;
	    show("slice (from const iov)", v);
	}
    }

    {
	p_("\n-- string_view");
	std::string vv = "abcdefghijklmn";
	std::string_view v;
	iov.iov_base = vv;
	p_("    string_view(v)");
	if (auto res = iov.string_view(v); !res) {
	    c7echo("\t%{}", res);
	} else {
	    check(iov, v.data(), v.size()+1);
	}
	p_("    string_view(v, 50) -> error");
	if (auto res = iov.string_view(v, 50); !res) {
	    c7echo("%{}", res);
	}
    }
}

int main()
{
    assign_test();
    get_test();
}

