#include <c7app.hpp>
#include <c7string/regex.hpp>


using c7::p_;
using c7::P_;


static bool check(const char *s)
{
    static c7::str::regex reg{R"--((ebsys|jeoleb|sys[1-5]\d|jeol[1-5]\d|mkebsys2?|pms))--"};
    return reg.match(s);
}

static bool check(const char *s, c7::strvec& m)
{
    static c7::str::regex reg{R"--((ebsys|jeoleb|sys([1-5]\d)|jeol([1-5]\d)|(mkebsys2?)|pms))--"};
    return reg.match(s, m);
}

int main(int argc, char **argv)
{
    std::vector<const char *> ccv = {
	"ebsys2",
	"sys47",
	"jeol61",
	"mkebsys",
	"mkebsys2",
	"jeol32",
	"jeoleb",
    };

    for (auto s: ccv) {
	c7::strvec match;
	p_("check('%{}')   -> %{}", s, check(s));
	P_("check('%{}',m) -> %{}", s, check(s, match));
	p_(", %{}", match);
    }
}
