#include <c7app.hpp>
#include <c7utils/time.hpp>
#include <c7json.hpp>


using c7::p_;
using c7::P_;


struct JsPos: public c7::json_object {
public:
    c7::json_int x;
    c7::json_int y;
    c7::json_real z;

    c7json_init(
	c7json_member(x),
	c7json_member(y),
	c7json_member(z),
	)
};


struct JsHis: public c7::json_object {
public:
    c7::json_usec time;
    JsPos pos;

    c7json_init(
	c7json_member(time),
	c7json_member(pos),
	)
};


struct MyData: public c7::json_object {
public:
    JsPos pos;
    c7::json_array<c7::json_str> books;
    c7::json_array<JsHis> trace;
    c7::json_bin bin;

    c7json_init_declare();
};

c7json_init_implement(
    MyData,
    c7json_member(pos),
    c7json_member(books),
    c7json_member(trace),
    c7json_member(bin),
    )


struct MyData2: public c7::json_object {
public:
    c7::json_array<JsHis> trace;
    c7::json_array<c7::json_str> books;
    c7::json_bin bin;
    JsPos pos2;

    c7json_init(
	c7json_member(trace),
	c7json_member(books),
	c7json_member(pos2),
	c7json_member(bin),
	)
};


struct MyData3: public c7::json_object {
    c7json_init()
};


int main(int argc, char **argv)
{
    MyData md;
    md.pos.x = 1;
    md.pos.y = 2;
    md.pos.z = 3.4;
    md.books.emplace_back("C++");
    md.books.emplace_back("Rust");
    md.books.emplace_back("\x01\x02\tOops");
    {
	auto& t = md.trace.extend();
	t.time = c7::time_us() - 100000000;
	t.pos.x = 11;
	t.pos.y = 22;
	t.pos.z = 33.44;
    }
    {
	auto& t = md.trace.extend();
	t.time = c7::time_us();
	t.pos.x = 111;
	t.pos.y = 222;
	t.pos.z = 333.444;
	p_("&md.trace[1]: %{p}", &t);
    }
    {
	auto& t = md.trace().back();
	p_("md.trace().back().pos %{},%{},%{}",
	   t.pos.x, t.pos.y, t.pos.z);
	p_("&md.trace[1]: %{p}", &t);
	t.pos.z = t.pos.z * 2.0;
    }

    md.bin = std::vector<uint8_t>{0,1,2,3,4};
    
    std::ostringstream out;
    c7::json_dump(md, out, 2);
    p_("md: <%{}>", out.str());

    //

    MyData2 md2;
    {
	c7::json_dump(md2, std::cout, 4);
	std::cout << '\n';
    }

    std::istringstream in2{out.str()};

    if (auto res = c7::json_load(md2, in2); !res) {
	c7error(res);
    }

    std::ostringstream out2;
    c7::json_dump(md2, out2, 3);
    p_("md2: out2 <%{}>", out2.str());
    P_("md2.bin: { ");
    for (auto u: md2.bin()) {
	P_("<%{}> ", static_cast<int>(u));
    }
    p_("}");

    //

    for (auto& b: md2.books) {
	p_("books: b.value:%{}", b());
    }
    p_("trace[1]: %{t}, x,y,z: %{}, %{}, %{}",
       md2.trace[1].time(), 
       md2.trace[1].pos.x(),
       md2.trace[1].pos.y(),
       md2.trace[1].pos.z());

    //

    MyData3 md3;
    {
	c7::json_dump(md3, std::cout, 4);
	std::cout << '\n';
    }

    std::istringstream in3{out.str()};
    if (auto res = c7::json_load(md3, in3); !res) {
	c7error(res);
    }

    std::ostringstream out3;
    c7::json_dump(md3, out3, 3);
    p_("md3: out3 <%{}>", out3.str());
}
