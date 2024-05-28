#include <c7app.hpp>
#include <c7event/shared_port.hpp>

using c7::p_;
using namespace c7::event;

int main()
{
    weak_port wp;
    {
	shared_port sp{2};
	sp.write("hello\n", 6);

	wp = sp;
	if (auto sp2 = wp.lock(); sp2) {
	    sp2.write("hello 2\n", 8);
	} else {
	    c7echo("sp2 is false");
	}
    }

    if (auto sp3 = wp.lock(); sp3) {
	sp3.write("hello 3\n", 8);
    } else {
	c7echo("sp3 is false");
    }
}
