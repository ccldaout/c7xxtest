#include <c7app.hpp>
#include <c7format.hpp>
#include <c7result.hpp>

using namespace std;
using namespace c7;
using c7::p_;

result<std::string> func3()
{
    static int n;
    if (n++ % 2 == 0)
	return result<std::string>("good morning");
    else
	return c7result_err(333, "func3");
}

result<std::string> func2()
{
    auto r = func3();
    p_("func2: result: %{}", r);
    if (r) {
	p_("func2: value:%{}", r.value());
	return r;
    }
    return c7result_err(std::move(r), "func%{}", 2);
}

result<> func1()
{
    auto r = func2();
    p_("func1: result: %{}", r);
    if (r)
	return result<>(std::move(r));
    else
	return c7result_err(std::move(r), 111, "func1");
}

result<> func0()
{
    auto r = func1();
    p_("func0: result: %{}", r);
    if (r)
	return result<>(std::move(r));
    else
	return c7result_err(std::move(r), ENOSPC);
}

result<> funcM()
{
    result<std::string> res0 = c7result_err(EINVAL, "1st error");
    result<std::string> res1 = c7result_err(ENOMEM, "end error");
    p_("-- 1");
    p_("res0: %{}", res0);
    p_("res1: %{}", res1);
    p_("-- 2 : res0.merge_iferror(std::move(res1))");
    res0.merge_iferror(std::move(res1));
    p_("res0: %{}", res0);
    p_("res1: %{}", res1);
    p_("-- 3 : res1 = std::move(res0)");
    res1 = std::move(res0);
    p_("res0: %{}", res0);
    p_("res1: %{}", res1);
    p_("-- 4");
    auto res3 = c7result_ok();
    auto res4 = c7result_ok();
    res3.merge_iferror(std::move(res4));
    p_("res3: %{}", res3);

    p_("-- 5");
    result<> res5 = c7result_err(EPERM);
    auto res6 = c7result_ok();
    res5.merge_iferror(std::move(res6));
    p_("res5: %{}", res5);

    p_("-- 6");
    result<> res7;
    res7 << c7result_ok()
	 << c7result_err(EINTR)
	 << c7result_ok()
	 << c7result_err(EPERM);
    p_("res7: %{}", res7);

    p_("-- return --");
    return res7;
}

static void test_X(size_t n)
{
    auto res = c7result_ok()
	>> ([n](       ){ return c7result_ok(n); })
	>> ([](auto&& v){ return (v % 2) == 0 ? c7result_err(EINVAL) : c7result_ok(v+300); })
	>> ([](auto&& v){ return c7result_ok(static_cast<double>(v)); })
	>> ([](auto&& v){ return c7result_ok(c7::format("<double:%{.3e}>", v)); });

    c7echo("res:%{}, value:%{}", bool(res), res.value("failure"));
}

static void test_exc()
{
    try {
	auto res1 = c7result_ok();
	res1.value();

	std::string s{"str"};
	c7::result<std::string> res2 = c7result_ok(s);
	p_("res2: %{}", res2.value());

	c7::result<int> res3 = c7result_err(EINVAL);
	p_("res3: %{}", res3.value());
    } catch (c7::result_exception& e) {
	auto err = e.as_result();
	p_("err: %{}", err);
    }
}


int main()
{
    for (int i = 0; i < 2; i++) {
	p_("--- #%{} ---", i);
	try {
	    auto r = func0();
	    if (!r) {
		p_("main: has error");
		//r.clear();
	    }
	    p_("main: result: %{}", r);
	    p_("main: has_what(111): %{}", r.has_what(111));
	} catch (std::exception& e) {
	    p_("exception: %{}", e.what());
	}
    }

    p_("--------------");
    {
	result<std::string> res = c7result_err(EINVAL, "error");
	p_("res.value: '%{}'", res.value("this is default"));

	res = c7result_ok<std::string>("success");
	p_("res.value: '%{}'", res.value("this is default"));
    }

    p_("--------------");
    {
	result<std::string> res0 = c7result_err(EINVAL, "error");
	result<std::string> res1 = c7result_err(std::move(res0), "add error");
	result<> res2;
	res1.copy(res2);
	p_("res0: %{}", res0);
	p_("res1: %{}", res1);
	p_("res2: %{}", res2);
    }

    p_("--------------");
    {
	p_("funcM: %{}", funcM());
    }

    p_("-------------- text_X --------------");
    {
	test_X(12);
	test_X(13);
    }

    p_("-------------- text_exc ------------");
    test_exc();

    p_("-------------- finish ------------");
    return 0;
}
