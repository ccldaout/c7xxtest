#include <c7app.hpp>
#include <c7nseq/range.hpp>
#include <c7nseq/vector.hpp>
#include <c7slice.hpp>


using c7::p_;
using c7::P_;
namespace ns = c7::nseq;


template <typename C>
static void show(const std::string& pref, const C& c)
{
    P_("%{}", pref);
    for (const auto& v: c) {
	P_(" %{}", v);
    }
    p_("");
}


int main()
{
    constexpr size_t N = 16;
    ptrdiff_t off = 1;
    ptrdiff_t gap = 3;
    size_t    n   = 6;

    p_("N::%{}, off:%{}, gap:%{}, n:%{}", N, off, gap, n);

    p_("vector<int>");
    {
	auto         iv = ns::range(N) | ns::to_vector();
	const auto &civ = iv;
	show(" iv", iv);

	auto sv = c7::make_slice(iv, off, gap, n);
	show(" sv", sv);

	c7::slice<const int> csv;
	csv = c7::make_slice(civ, off, gap, n);
	show("csv", csv);

	p_(" += 100");
	for (auto& v: sv) {
	    v += 100;
	}
	show(" sv", sv);
	show(" iv", iv);
    }

    p_("array<int,16>");
    {
	std::array<int, N>         iv{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	const std::array<int, N>& civ{iv};
	show(" iv", iv);

	auto sv = c7::make_slice(iv, off, gap, n);
	show(" sv", sv);

	c7::slice<const int> csv;
	csv = c7::make_slice(civ, off, gap, n);
	show("csv", csv);

	p_(" += 100");
	for (auto& v: sv) {
	    v += 100;
	}
	show(" sv", sv);
	show(" iv", iv);
    }

    p_("int[16]");
    {
	int        iv[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	const int civ[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

	auto sv = c7::make_slice(iv, off, gap, n);
	show(" sv", sv);

	c7::slice<const int> csv;
	csv = c7::make_slice(civ, off, gap, n);
	show("csv", csv);

	p_(" += 100");
	for (auto& v: sv) {
	    v += 100;
	}
	show(" sv", sv);
	show(" iv", iv);
    }

    p_("int*");
    {
	int        iv[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	const int civ[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

	auto sv = c7::make_slice(&iv[0], N, off, gap, n);
	show(" sv", sv);

	c7::slice<const int> csv;
	csv = c7::make_slice(&civ[0], N, off, gap, n);
	show("csv", csv);

	p_(" += 100");
	for (auto& v: sv) {
	    v += 100;
	}
	show(" sv", sv);
	show(" iv", iv);
    }

    p_("convert");
    {
	auto iv = ns::range(10) | ns::to_vector();
	auto sv = c7::make_slice(iv, off, gap, n);
	show("\t    iv ", iv);
	show("\t    sv ", sv);
	{
	    auto v = sv.convert<std::vector>();
	    show("\tvector ", v);
	}
	{
	    auto v = sv.convert<std::list>();
	    show("\t  list ", v);
	}

	const auto& csv{sv};
	{
	    auto v = csv.convert<std::vector>();
	    show("\tvector (from const slice&)", v);
	}
    }

    p_("convert<std::string>");
    {
	auto iv = std::vector<char>{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};
	auto sv = c7::make_slice(iv, off, gap, n);
	show("\t    iv ", iv);
	show("\t    sv ", sv);
	{
	    auto v = sv.convert<std::string>();
	    show("\tstring ", v);
	}

	const auto& csv{sv};
	{
	    auto v = csv.convert<std::string>();
	    show("\tstring (from const slice&)", v);
	}
    }

    p_("back_insert & copy");
    {
	auto iv  = ns::range(10) | ns::to_vector();
	auto sv1 = c7::make_slice(iv, off, gap, n);
	auto sv2 = c7::make_slice(iv, off+1, gap, n);
	auto sv3 = c7::make_slice(iv, off+2, gap, n);
	show("\t     iv  ", iv);
	show("\t     sv1 ", sv1);
	show("\t     sv2 ", sv2);
	show("\t     sv3 ", sv3);

	{
	    std::vector<int> v2;
	    show("\t       v2  ", v2);

	    sv1.copy_to(v2);
	    show("\tsv1 >> v2  ", v2);

	    auto it = sv2.copy_to(v2);
	    show("\tsv2 >> v2  ", v2);
	    it = sv3.copy_to(it);
	    show("\tsv3 >> v2  ", v2);
	}

	{
	    const auto& csv1{sv1};
	    const auto& csv2{sv2};
	    const auto& csv3{sv3};

	    std::list<int> v2;
	    show("\t       v2  ", v2);

	    csv1.copy_to(v2);
	    show("\tcsv1 >> v2  ", v2);

	    auto it = csv2.copy_to(v2);
	    show("\tcsv2 >> v2  ", v2);
	    it = csv3.copy_to(it);
	    show("\tcsv3 >> v2  ", v2);
	}
    }
}
