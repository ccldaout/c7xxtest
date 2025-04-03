//#define C7FORMAT_EX

#include <c7format/format_r1.hpp>
#include <c7format/format_r2.hpp>
#include <c7format/format_r3.hpp>
#include <c7seq.hpp>
#include <c7utils.hpp>
#include <iostream>
#include <vector>
#include <typeinfo>
#include <ctime>


using namespace std;


template <typename... Args>
void P_(const char *fmt, const Args&... args)
{
    c7::format_r1::p_(fmt, args...);
    c7::format_r2::p_(fmt, args...);
    c7::format_r3::p_(fmt, args...);
}

template <size_t N, typename... Args>
void F_(std::string& s, const char (&fmt)[N], const Args&... args)
{
    c7::format_r1::format(s, fmt, args...);
    c7::format_r2::format(s, fmt, args...);
    c7::format_r3::format(s, fmt, args...);
}


template <typename T>
struct c7::format_traits<vector<T>> {
    static inline void convert(std::ostream& out, const std::string& format,
			       const vector<T>& vec) {
	out << "{ ";
	for (auto& v: vec) {
	    out << v << " ";
	}
	out << "}";
    }
};

struct xyvec {
    int x = 0;
    int y = 0;
    void print(std::ostream& out, const std::string&) const {
	c7::format(out, "xyvec(%{}, %{})", x, y);
    }
};

namespace test {
struct xyvec2 {
    int x = 0;
    int y = 0;
};
void print_type(std::ostream& out, const std::string&, const xyvec2& v)
{
    c7::format(out, "print_type: xyvec2(%{}, %{})", v.x, v.y);
}
} // test


namespace enum_test {
enum old_enum {
    HELLO, WORLD,
};

void print_type(std::ostream& out, const std::string&, old_enum e)
{
    switch (e) {
    case HELLO:
	out << "HELLO"; break;
    case WORLD:
	out << "WORLD"; break;
    }
}

enum class new_enum {
    HELLO, WORLD,
};

void print_type(std::ostream& out, const std::string&, enum_test::new_enum e)
{
    switch (e) {
    case enum_test::new_enum::HELLO:
	out << "HELLO"; break;
    case enum_test::new_enum::WORLD:
	out << "WORLD"; break;
    }
}

enum class nopr_enum {
    HELLO = 0xf0, WORLD = 0x81,
};
}


class intwrap {
private:
    int value_;
public:
    intwrap(int v): value_(v) {}
    int print_as() const { return value_; };
};


int main()
{
    P_("age: %{}, name: %{_<*} ... lang:%{}++", 10, 40, "hello world", 'C');

    const string s("Masakazu Iwanaga");
    P_("My name is %{}. weight: %{*.*f}, pointer:%{}", s, 4, 1, 63.4, (void*)&s);

    vector<int> v{ 1, 2, 3 };
    P_("vector: %{}", v);

    vector<std::string> v2{ "This", "is", "a", "pen !" };
    P_("vector<str>: %{>_30} %{#x} %{d}", v2, (short)-3276, true);

    P_("int & {}: %{o}[8] %{b9}[2]", 123, 123);

    P_("rate: %{}%% and %{}% ...", 17, 99);

    auto tv = std::time(nullptr);
    P_("tv(s) : %{T} !!", tv);
    P_("tv(us): %{t%m/%d %H:%M.%S} !!!", c7::time_us());

    enum_test::old_enum oldenum = enum_test::WORLD;
    P_("old enum %{}", oldenum);

    enum_test::new_enum newenum = enum_test::new_enum::WORLD;
    P_("new enum %{}", newenum);

    P_("nopr_enum: %{}, %{} / %{b} %{b}",
       enum_test::nopr_enum::HELLO,
       enum_test::nopr_enum::WORLD,
       enum_test::nopr_enum::HELLO,
       enum_test::nopr_enum::WORLD);

    c7::com_status ios = c7::com_status::TIMEOUT;
    P_("com_status (new enum) %{}", ios);

    uint8_t u8 = 33;
    P_("uint8_t: <%{}> <%{d}> <%{x}>", u8, u8, u8);

    int X = 'X';
    P_("int('X'): <%{}> <%{c}>", X, X);

    P_("too few %{}_%{}_%{}_data", 1);
    P_("too many %{} data", 1, 2, 3);

    xyvec xy{11, 22};
    P_("xy: <%{}>", xy);

    test::xyvec2 xy2{77,88};
    P_("xy2: <%{}>", xy2);

    intwrap iw{567};
    P_("iw: %{#08x}", iw);

    P_("--------------------------------------------------------");

    int16_t val = -123;
    double d = 2.0000123456789;
    P_("#8x:[%{#8x}] #<8x:[%{#<8x}] #<_8x:[%{#<_8x}] #08x:[%{#08x}]",
       val, val, val, val);
    P_("<8d:[%{<8d}] +<8d:[%{+<8d}] +8d:[%{+8d}] +=8d:[%{+=8d}]",
       val, val, val, val);
    P_(":[%{}], f:[%{f}] .10f:[%{.10f}] .0f:[%{.0f}] 8.5f:[%{8.5f}] e:[%{e}]",
       d, d, d, d, d, d);

    P_("--------------------------------------------------------");
    {
	std::string s;
	F_(s, "#8x:[%{#8x}] #<8x:[%{#<8x}] #<_8x:[%{#<_8x}] #08x:[%{#08x}]\n",
	   val, val, val, val);
	F_(s, "<8d:[%{<8d}] +<8d:[%{+<8d}] +8d:[%{+8d}] +=8d:[%{+=8d}]\n",
	   val, val, val, val);
	F_(s, ":[%{}], f:[%{f}] .10f:[%{.10f}] .0f:[%{.0f}] 8.5f:[%{8.5f}] e:[%{e}]\n",
	   d, d, d, d, d, d);
	std::cout << s;
    }

    return 0;
}
