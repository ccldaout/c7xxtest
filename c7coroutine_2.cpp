#include <c7app.hpp>
#include <c7nseq/string.hpp>
#include <c7coroutine_r2.hpp>


using c7::p_;


class my_co: public c7::r2::coroutine<> {
public:
    void entry_point() override {
	bool b;
	c7echo("%{} 1 (enter)", __func__);
	b = co_switch();
	c7echo("%{} %{}", __func__, b);
	c7echo("%{} 2", __func__);
	b = co_switch();
	c7echo("%{} %{}", __func__, b);
	c7echo("%{} 3", __func__);
	b = co_switch();
	c7echo("%{} %{}", __func__, b);
	c7echo("%{} 4", __func__);
	b = co_switch();
	c7echo("%{} %{} (exit)", __func__, b);
    }
};


int main()
{
    my_co co;
    bool b;
    c7echo("%{} 1 (start)", __func__);
    b = co.start();
    c7echo("%{} %{} (started)", __func__, b);
    c7echo("%{} 2", __func__);
    b = co.resume();
    c7echo("%{} %{}", __func__, b);
    c7echo("%{} 3", __func__);
    b = co.resume();
    c7echo("%{} %{}", __func__, b);
    c7echo("%{} 4", __func__);
    b = co.resume();
    c7echo("%{} %{}", __func__, b);
    c7echo("%{} 5", __func__);
    b = co.resume();
    c7echo("%{} %{}", __func__, b);
    c7echo("%{} 6", __func__);
    b = co.resume();
    c7echo("%{} %{}", __func__, b);
    c7echo("%{} 7", __func__);
    b = co.resume();
    c7echo("%{} %{}", __func__, b);
}
