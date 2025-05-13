#include <c7app.hpp>
#include <c7utils/time.hpp>

using c7::p_;

int main()
{
    auto tv =
	c7::make_usec()
	.now()
	.year(2011)
	.month(7)
	.mday(21)
	.hour(9)
	.min(42)
	.make();
    p_("%{t%Y %m/%d %H:%M:%S%z}", tv);
}
