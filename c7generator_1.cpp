#include <c7format.hpp>
#include <c7coroutine.hpp>
#include <cstring>


using c7::generator;
using c7::p_;


/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/

struct data_t {
    int index;
    std::string s;
};

static void makedata()
{
    for (int i = 0; i < 5; i++) {
	generator<data_t>::yield(data_t{ i, c7::format("data%{}", i) });
    }
}

static void makedata2()
{
    std::vector<data_t> vec;
    for (int i = 0; i < 5; i++) {
	vec.push_back(data_t{ i, c7::format("data%{}", i) });
    }
    for (auto& v: vec) {
	generator<data_t>::yield(v);
    }
}

static void makedata3(std::vector<data_t>& vec)
{
    for (auto& v: vec) {
	generator<data_t&>::yield(v);
    }
}

static void test_generator()
{
    p_("------------------------------");
    p_("test_generator start");

    {
	generator<std::string> gen(
	    1,
	    []() {
		using gen = generator<std::string>;
		gen::yield("These");
		gen::yield("data");
		gen::yield("are");
		gen::yield("from");
		gen::yield("generator.");
	    });

	for (auto& s: gen) {
	    p_("<%{}>", s);
	}
	p_("is_success: %{}", gen.is_success());
    }

    p_("----");
    {
	int i = 0;
	generator<std::string> gen(
	    1,
	    []() {
		using gen = generator<std::string>;
		gen::yield("These");
		gen::yield("data");
		gen::yield("are");
		gen::yield("from");
		gen::yield("generator.");
	    });

	for (auto& s: gen) {
	    p_("<%{}>", s);
	    if (i++ == 2)
		break;
	}
	p_("is_success: %{}", gen.is_success());
    }

    p_("---- int literal");
    {
	generator<int> gen(
	    1,
	    []() {
		using gen = generator<int>;
		gen::yield(1);
		gen::yield(2);
		gen::yield(3);
	    });

	for (auto& n: gen) {
	    p_("<%{}, %{}>", &n, n);
	}
    }

    p_("---- vec<int>");
    {
	generator<int> gen(
	    1,
	    []() {
		std::vector vec{1,2,3};
		using gen = generator<int>;
		for (auto& v: vec) {
		    gen::yield(v);
		}
	    });

	for (auto& n: gen) {
	    p_("<%{}, %{}>", &n, n);
	}
    }

    p_("---- vec<int> & gen<int&>");
    {
	std::vector vec{1,2,3};
	generator<int&> gen(
	    1,
	    [&vec]() {
		using gen = generator<int&>;
		for (auto& v: vec) {
		    gen::yield(v);
		}
	    });

	for (auto& n: gen) {
	    p_("<%{}, %{}>", &n, n);
	    n *= 2;
	}
	for (auto& v: vec) {
	    p_("vec: %{} %{}", &v, v);
	}
    }

    p_("---- makedata ");
    {
	for (auto& idx: generator<data_t>(1, makedata)) {
	    p_("idx:%{}, index:%{}, s:%{}", &idx, idx.index, idx.s);
	}
    }

    p_("---- makedata2 ");
    {
	for (auto& idx: generator<data_t>(1, makedata2)) {
	    p_("idx:%{}, index:%{}, s:%{}", &idx, idx.index, idx.s);
	    //idx.index *= 2;	// ERROR
	}
    }

    p_("---- makedata3");
    {
	std::vector<data_t> vec;
	for (int i = 0; i < 5; i++) {
	    vec.push_back(data_t{ i, c7::format("data%{}", i) });
	}
	for (auto& idx: generator<data_t&>(1, [&vec](){makedata3(vec);})) {
	    p_("idx:%{}, index:%{}, s:%{}", &idx, idx.index, idx.s);
	    idx.index *= 2;
	}
	p_(" - - - ");
	for (auto& v: vec) {
	    p_("vec:%{}, index:%{}, s:%{}", &v, v.index, v.s);
	}
    }

    p_("test_generator end");
}


/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/

int main()
{
    test_generator();
    return 0;
}
