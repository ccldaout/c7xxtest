#include <c7app.hpp>
#include <c7utils/auto_restore.hpp>

using c7::p_;

struct data_t {
    int x;
    double y;
    const char *s;
};

void func(data_t& data)
{
    data.x = 99;
    data.y = 3.14;
    data.s = "after";
}

int main()
{
    data_t data;
    data.x = 1;
    data.y = -1.2;
    data.s = "before";

    p_(" before: x:%{}, y:%{}, s:%{}", data.x, data.y, data.s);
    c7auto_restore(data.x, data.s) {
	func(data);
	p_("changed: x:%{}, y:%{}, s:%{}", data.x, data.y, data.s);
    }
    p_("restore: x:%{}, y:%{}, s:%{}", data.x, data.y, data.s);
}
