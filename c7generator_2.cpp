#include <c7format.hpp>
#include <c7generator_r2.hpp>
#include <cstring>


using namespace c7::r2;
using c7::p_;


template <typename Output>
using gen_output = typename generator<Output>::gen_output;


/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/

struct data_t {
    int index;
    std::string s;
};

static void makedata(gen_output<data_t>& out)
{
    for (int i = 0; i < 5; i++) {
	out << data_t{ i, c7::format("data%{}", i) };
    }
}

static void makedata2(gen_output<data_t>& out)
{
    std::vector<data_t> vec;
    for (int i = 0; i < 5; i++) {
	vec.push_back(data_t{ i, c7::format("data%{}", i) });
    }
    for (auto& v: vec) {
	out << v;
    }
}

static void makedata3(gen_output<data_t&>& out)
{
    std::vector<data_t> vec;
    for (int i = 0; i < 5; i++) {
	vec.push_back(data_t{ i, c7::format("data%{}", i) });
    }
    for (auto& v: vec) {
	out << v;
    }
    p_(" - - - ");
    for (auto& v: vec) {
	p_("v:%{}, index:%{}, s:%{}", &v, v.index, v.s);
    }
}

static void test_generator()
{
    p_("------------------------------");
    p_("test_generator start");

    {
	generator<std::string> gen{1};
	gen.start(
	    [](auto& out) {
		out << "These";
		out << "data";
		out << "are";
		out << "from";
		out << "generator.";
	    });
	for (auto& s: gen) {
	    p_("<%{}>", s);
	}
	//p_("is_success: %{}", gen.is_success());
    }

    p_("----");
    {
	int i = 0;
	generator<std::string> gen{1};
	gen.start(
	    [](auto& out) {
		out << "These";
		out << "data";
		out << "are";
		out << "from";
		out << "generator.";
	    });
	for (auto& s: gen) {
	    p_("<%{}>", s);
	    if (i++ == 2)
		break;
	}
	//p_("is_success: %{}", gen.is_success());
    }

    p_("---- int literal");
    {
	generator<int> gen{1};
	gen.start(
	    [](auto& out) {
		out << 1;
		out << 2;
		out << 3;
	    });
	for (auto& n: gen) {
	    p_("<%{}, %{}>", &n, n);
	}
    }

    p_("---- vec<int>");
    {
	generator<int> gen{1};
	gen.start(
	    [](auto& out) {
		std::vector vec{1,2,3};
		for (auto& v: vec) {
		    out << v;
		}
	    });
	for (auto& n: gen) {
	    p_("<%{}, %{}>", &n, n);
	}
    }

    p_("---- vec<int> & gen<int&>");
    {
	generator<int&> gen{1};
	gen.start(
	    [](auto& out) {
		std::vector vec{1,2,3};
		for (auto& v: vec) {
		    out << v;
		}
		for (auto& v: vec) {
		    p_("vec: %{} %{}", &v, v);
		}
	    });
	for (auto& n: gen) {
	    p_("<%{}, %{}>", &n, n);
	    n *= 2;
	}
    }

    p_("---- makedata ");
    {
	for (auto& idx: generator<data_t>{1}.start(makedata)) {
	    p_("idx:%{}, index:%{}, s:%{}", &idx, idx.index, idx.s);
	}
    }

    p_("---- makedata2 ");
    {
	for (auto& idx: generator<data_t>{1}.start(makedata2)) {
	    p_("idx:%{}, index:%{}, s:%{}", &idx, idx.index, idx.s);
	    //idx.index *= 2;	// ERROR
	}
    }

    p_("---- makedata3");
    {
	for (auto& idx: generator<data_t&>{1}.start(makedata3)) {
	    p_("idx:%{}, index:%{}, s:%{}", &idx, idx.index, idx.s);
	    idx.index *= 2;
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
