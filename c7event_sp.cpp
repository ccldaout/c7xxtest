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

	p_("before wp = sp;");
	p_("wp:%{}", wp);
	p_("sp == wp:%{}", sp == wp);
	p_("wp == sp:%{}", wp == sp);
	p_("sp != wp:%{}", sp != wp);
	p_("wp != sp:%{}", wp != sp);

	wp = sp;

	p_("after wp = sp;");
	p_("wp:%{}", wp);
	p_("sp == wp:%{}", sp == wp);
	p_("wp == sp:%{}", wp == sp);
	p_("sp != wp:%{}", sp != wp);
	p_("wp != sp:%{}", wp != sp);

	if (auto sp2 = wp.lock(); sp2) {
	    sp2.write("hello 2\n", 8);
	    p_("after sp2 = wp.lock;");
	    p_("sp2 == wp :%{}", sp2 == wp);
	    p_("wp  == sp2:%{}", wp == sp2);
	    p_("sp2 != wp :%{}", sp2 != wp);
	    p_("wp  != sp2:%{}", wp != sp2);
	    p_("sp2 == sp :%{}", sp2 == sp);
	    p_("sp2 != sp :%{}", sp2 != sp);

	    shared_port sp3{3};
	    p_("sp2 == sp3 :%{}", sp2 == sp3);
	    p_("sp2 != sp3 :%{}", sp2 != sp3);

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
