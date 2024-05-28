#include <c7app.hpp>
#include <c7slice.hpp>
#include <c7format/helper.hpp>
#include <c7nseq/range.hpp>
#include <c7nseq/vector.hpp>

using c7::p_;
namespace ns = c7::nseq;

int main()
{
    auto vec = ns::range(30) | ns::to_vector();
    p_("%{}", vec);
    {
	ptrdiff_t off = 2;
	ptrdiff_t gap = 3;
	auto sl = c7::make_slice(vec, off, gap);
	p_("sl(vec, off:%{}, gap:%{}) %{}", off, gap, sl);
	{
	    ptrdiff_t off = 1;
	    ptrdiff_t gap = 2;
	    auto ss = c7::make_slice(sl, off, gap);
	    p_("ss(sl, off:%{}, gap:%{}) %{}", off, gap, ss);
	}
	{
	    ptrdiff_t off = 1;
	    ptrdiff_t gap = 2;
	    size_t n = 3;
	    auto ss = c7::make_slice(sl, off, gap, n);
	    p_("ss(sl, off:%{}, gap:%{}, n:%{}) %{}", off, gap, n, ss);
	}
	{
	    ptrdiff_t off = 5;
	    ptrdiff_t gap = -2;
	    auto ss = c7::make_slice(sl, off, gap);
	    p_("ss(sl, off:%{}, gap:%{}) %{}", off, gap, ss);
	}
	{
	    ptrdiff_t off = 5;
	    ptrdiff_t gap = -2;
	    size_t n = 2;
	    auto ss = c7::make_slice(sl, off, gap, n);
	    p_("ss(sl, off:%{}, gap:%{}, n:%{}) %{}", off, gap, n, ss);
	}
	{
	    ptrdiff_t off = -5;
	    ptrdiff_t gap = -2;
	    auto ss = c7::make_slice(sl, off, gap);
	    p_("ss(sl, off:%{}, gap:%{}) %{}", off, gap, ss);
	}
    }
    {
	ptrdiff_t off = 28;
	ptrdiff_t gap = -3;
	auto sl = c7::make_slice(vec, off, gap);
	p_("sl(vec, off:%{}, gap:%{}) %{}", off, gap, sl);
	{
	    ptrdiff_t off = 1;
	    ptrdiff_t gap = 2;
	    auto ss = c7::make_slice(sl, off, gap);
	    p_("ss(sl, off:%{}, gap:%{}) %{}", off, gap, ss);
	}
	{
	    ptrdiff_t off = 1;
	    ptrdiff_t gap = 2;
	    size_t n = 3;
	    auto ss = c7::make_slice(sl, off, gap, n);
	    p_("ss(sl, off:%{}, gap:%{}, n:%{}) %{}", off, gap, n, ss);
	}
	{
	    ptrdiff_t off = 5;
	    ptrdiff_t gap = -2;
	    auto ss = c7::make_slice(sl, off, gap);
	    p_("ss(sl, off:%{}, gap:%{}) %{}", off, gap, ss);
	}
	{
	    ptrdiff_t off = 5;
	    ptrdiff_t gap = -2;
	    size_t n = 2;
	    auto ss = c7::make_slice(sl, off, gap, n);
	    p_("ss(sl, off:%{}, gap:%{}, n:%{}) %{}", off, gap, n, ss);
	}
	{
	    ptrdiff_t off = -5;
	    ptrdiff_t gap = -2;
	    auto ss = c7::make_slice(sl, off, gap);
	    p_("ss(sl, off:%{}, gap:%{}) %{}", off, gap, ss);
	}
    }
}
