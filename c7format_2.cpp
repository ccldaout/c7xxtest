#include <iomanip>
#include <sstream>
#include <c7app.hpp>
#include <c7format.hpp>
#include <c7strmbuf/hybrid.hpp>
#include <c7strmbuf/strref.hpp>
#include <c7utils.hpp>


#define F_(os_, fmt_, ...)	({static c7::analyzed_format fmts{fmt_}; f_(os_, fmts, __VA_ARGS__);})
#define f_ c7::format
#define p_ c7::p_
#define C_ std::sprintf


int main(int argc, char **argv)
{
    int n = std::stoi(argv[1]);
    int16_t val = -123;
    double d = 2.0000123456789;
    c7::usec_t beg;

    if (argc != 2) {
	c7exit("Usage: c7fomrat_2 count");
    }

    beg = c7::time_us();
    for (int i = 0; i < n; i++) {
	std::stringstream s;
	s << std::showbase << std::internal << std::hex << std::setw(8) << val
	  << std::showbase << std::left     << std::hex << std::setw(8) << val
	  << std::showbase << std::left     << std::setfill('_') << std::hex << std::setw(8) << val
	  << std::showbase << std::internal << std::setfill('0') << std::hex << std::setw(8) << val;

	s << std::left     << std::setw(8) << std::dec << val
	  << std::left     << std::setw(8) << std::dec << std::showpos << val
	  << std::right    << std::setw(8) << std::dec << val
	  << std::internal << std::setw(8) << std::dec << std::showpos << val;

	s << std::left << std::defaultfloat << d
	  << std::left << std::fixed        << d
	  << std::left << std::fixed << std::setprecision(10) << d
	  << std::left << std::fixed << std::setprecision(0) << d
	  << std::left << std::fixed << std::setprecision(5) << std::setw(8) << d
	  << std::left << std::scientific << d;
    }
    p_("     iomanip: %{t}", c7::time_us() - beg);

    beg = c7::time_us();
    for (int i = 0; i < n; i++) {
	std::stringstream s;
	f_(s, "#8x:[%{#8x}] #<8x:[%{#<8x}] #<_8x:[%{#<_8x}] #08x:[%{#08x}]",
	   val, val, val, val);
	f_(s, "<8d:[%{<8d}] +<8d:[%{+<8d}] +8d:[%{+8d}] +=8d:[%{+=8d}]",
	   val, val, val, val);
	f_(s, ":[%{}], f:[%{f}] .10f:[%{.10f}] .0f:[%{.0f}] 8.5f:[%{8.5f}] e:[%{e}]",
	   d, d, d, d, d, d);
    }
    p_("stringstream: %{t}", c7::time_us() - beg);

    beg = c7::time_us();
    for (int i = 0; i < n; i++) {
	auto b = c7::strmbuf::hybrid();
	auto s = std::basic_ostream(&b);
	f_(s, "#8x:[%{#8x}] #<8x:[%{#<8x}] #<_8x:[%{#<_8x}] #08x:[%{#08x}]",
	   val, val, val, val);
	f_(s, "<8d:[%{<8d}] +<8d:[%{+<8d}] +8d:[%{+8d}] +=8d:[%{+=8d}]",
	   val, val, val, val);
	f_(s, ":[%{}], f:[%{f}] .10f:[%{.10f}] .0f:[%{.0f}] 8.5f:[%{8.5f}] e:[%{e}]",
	   d, d, d, d, d, d);
    }
    p_(" strm/hybrid: %{t}", c7::time_us() - beg);

    beg = c7::time_us();
    for (int i = 0; i < n; i++) {
	auto b = c7::strmbuf::hybrid();
	auto s = std::basic_ostream(&b);
	F_(s, "#8x:[%{#8x}] #<8x:[%{#<8x}] #<_8x:[%{#<_8x}] #08x:[%{#08x}]",
	   val, val, val, val);
	F_(s, "<8d:[%{<8d}] +<8d:[%{+<8d}] +8d:[%{+8d}] +=8d:[%{+=8d}]",
	   val, val, val, val);
	F_(s, ":[%{}], f:[%{f}] .10f:[%{.10f}] .0f:[%{.0f}] 8.5f:[%{8.5f}] e:[%{e}]",
	   d, d, d, d, d, d);
    }
    p_("#strm/hybrid: %{t}", c7::time_us() - beg);

    beg = c7::time_us();
    for (int i = 0; i < n; i++) {
	f_("#8x:[%{#8x}] #<8x:[%{#<8x}] #<_8x:[%{#<_8x}] #08x:[%{#08x}]",
	   val, val, val, val);
	f_("<8d:[%{<8d}] +<8d:[%{+<8d}] +8d:[%{+8d}] +=8d:[%{+=8d}]",
	   val, val, val, val);
	f_(":[%{}], f:[%{f}] .10f:[%{.10f}] .0f:[%{.0f}] 8.5f:[%{8.5f}] e:[%{e}]",
	   d, d, d, d, d, d);
    }
    p_("    string  : %{t}", c7::time_us() - beg);

    beg = c7::time_us();
    for (int i = 0; i < n; i++) {
	f_("#8x:[%{#8x}] #<8x:[%{#<8x}] #<_8x:[%{#<_8x}] #08x:[%{#08x}]"
	   "<8d:[%{<8d}] +<8d:[%{+<8d}] +8d:[%{+8d}] +=8d:[%{+=8d}]"
	   ":[%{}], f:[%{f}] .10f:[%{.10f}] .0f:[%{.0f}] 8.5f:[%{8.5f}] e:[%{e}]",
	   val, val, val, val,
	   val, val, val, val,
	   d, d, d, d, d, d);
    }
    p_("    string#2: %{t}", c7::time_us() - beg);

    beg = c7::time_us();
    for (int i = 0; i < n; i++) {
	std::string str;
	auto b = c7::strmbuf::strref(str);
	auto s = std::basic_ostream(&b);
	f_(s, "#8x:[%{#8x}] #<8x:[%{#<8x}] #<_8x:[%{#<_8x}] #08x:[%{#08x}]",
	   val, val, val, val);
	f_(s, "<8d:[%{<8d}] +<8d:[%{+<8d}] +8d:[%{+8d}] +=8d:[%{+=8d}]",
	   val, val, val, val);
	f_(s, ":[%{}], f:[%{f}] .10f:[%{.10f}] .0f:[%{.0f}] 8.5f:[%{8.5f}] e:[%{e}]",
	   d, d, d, d, d, d);
    }
    p_(" strm/strref: %{t}", c7::time_us() - beg);

    beg = c7::time_us();
    for (int i = 0; i < n; i++) {
	char buff[512];
	C_(buff, "#8x:[%#8hx] #<8x:[%#-8hx] #<_8x:[%#-8hx] #08x:[%#08hx]",
	   val, val, val, val);
	C_(buff, "<8d:[%-8hd] +<8d:[%+-8hd] +8d:[%+8d] +=8d:[%+8d]",
	   val, val, val, val);
	C_(buff, ":[%g], f:[%f] .10f:[%.10f] .0f:[%.0f] 8.5f:[%8.5f] e:[%e]",
	   d, d, d, d, d, d);
    }
    p_("     sprintf: %{t}", c7::time_us() - beg);

    return 0;
}
