#include <c7defer.hpp>
#include <c7format.hpp>
#include <c7utils.hpp>
#include <memory>
#include <iostream>


using namespace std;
using namespace c7;
using c7::p_;


static void defer_test1()
{
    p_("--------------------------------------");

    {
	p_("entered block");
	c7::defer def1([](){ p_("defered code !"); });
	p_("exit block ...");
    }
    p_("exited block");
    p_("--");
    {
	p_("entered block");
	c7::defer def1([](){ p_("defered code !"); });
	{
	    p_("  entered block {");
	    c7::defer def2(std::move(def1));
	    p_("  exit block ...}");
	}
	p_("exit block ...");
    }
    p_("exited block");
    p_("--");
    {
	p_("entered block");
	c7::defer def1([](){ p_("defered code !"); });
	{
	    p_("  entered block {");
	    c7::defer def3;
	    def3 = std::move(def1);
	    p_("  exit block ...}");
	}
	p_("exit block ...");
    }
    p_("exited block");

    p_("--");
    p_("defer bool ...");
    c7::defer def;
    p_("defer bool : %{} %{}", bool(def), def ? true : false);
    p_("def = [](){ ... }");
    def = [](){ p_("defer called"); };
    p_("defer bool : %{}", bool(def));
    def();
    p_("defer bool : %{}", bool(def));
    p_("def = [](){ ... }");
    def = [](){ p_("defer called"); };
    p_("def.cancel()");
    def.cancel();
    p_("defer bool : %{}", bool(def));
}


static void defer_test2b()
{
    defer d([](){ p_("defer test2b"); });
    throw exception();
}

static void defer_test2a()
{
    defer d([](){ p_("defer test2a"); });
    {
	defer d2([](){ p_("defer test2a.2"); });
	defer_test2b();
    }
}

static void defer_test2()
{
    p_("--------------------------------------");

    defer d([](){ p_("defer test2"); });
    {
	defer d2([](){ p_("defer test2.2"); });
	defer_test2a();
    }
}

static void defer_test3()
{
    p_("--------------------------------------");

    auto on_return = defer([](){ p_("defer test3 #1"); });
    on_return += [](){ p_("defer test3 #2"); };
    on_return += [](){ p_("defer test3 #3"); };
}

static void defer_test4()
{
    p_("--------------------------------------");

    for (int i = 0; i < 5; i++) {
	auto on_return = defer([i](){ p_("defer test4 for#%{}", i); });
	if (i % 2 == 0) {
	    continue;
	}
	p_("odd");
    }
    int i = 0;
    while (i < 5) {
	auto on_return = defer([i](){ p_("defer test4 while#%{}", i); });
	if (i++ % 2 == 0) {
	    continue;
	}
	p_("odd");
    }
}

int main()
{
    defer_test1();
    try {
	defer_test2();
    } catch (...) {
    }
    defer_test3();
    defer_test4();
    return 0;
}
