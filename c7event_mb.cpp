#define C7_IOVEC_PROXY_DEBUG
#include <c7app.hpp>
#include <c7event/msgbuf_impl.hpp>


using c7::p_;


struct my_header {
    int32_t event;
    void print(std::ostream& o, const std::string&) const {
	c7::format(o, "%{}", event);
    }
};
struct my_header2 {
    int32_t event;
    int32_t event2;
    void print(std::ostream& o, const std::string&) const {
	c7::format(o, "%{}", event);
    }
};

typedef c7::event::multipart_msgbuf<my_header, 5> ipc_msgbuf;
typedef c7::event::multipart_msgbuf<my_header2, 5> ipc_msgbuf2;
static ipc_msgbuf test_buffer;


struct xy_t {
    int x, y;
};


int main(int argc, char **argv)
{
    test_buffer.header.event = 11;
    auto v = test_buffer[3];
    v.iov_base = "hello world";
    test_buffer.dump();

    p_("[3]: '%{}'", test_buffer[3].iov_base.as<char>());

    xy_t xy { 11, 22 };
    p_("xy <%{}, %{}>", xy.x, xy.y);
    test_buffer[1].iov_base = &xy;
    test_buffer.dump();

    xy_t *xyp = test_buffer[1].iov_base;
    p_("[1]: x:%{}, y:%{}", xyp->x, xyp->y);
    xyp->x *= 3;

    auto v2 = test_buffer[1];
    xy_t *xyp2 = v2.iov_base;
    xyp2->y += 1000;
    p_("[1]: x:%{}, y:%{}", xyp->x, xyp->y);

    p_("------------ buf1 (default construct)");
    ipc_msgbuf buf1;
    buf1.dump();

    p_("------------ buf2 = test_buffer.deep_cop()");
    auto buf2 = test_buffer.deep_copy();
    buf2.dump();
    xyp = buf2[1].iov_base;
    p_("[1]: x:%{}, y:%{}", xyp->x, xyp->y);

    p_("------------ buf3 = move(buf2)");
    auto buf3 = std::move(buf2);
    p_("buf2");
    buf2.dump();
    p_("buf3");
    buf3.dump();

    p_("------------ test_buffer.deep_copy_from(buf3)");
    std::string ss {"c++"};
    buf3[5].iov_base = ss;
    test_buffer.deep_copy_from(buf3);
    p_("test_buffer");
    test_buffer.dump();
    p_("buf3");
    buf3.dump();
    char *s5 = test_buffer[5].iov_base;
    p_("[5]: '%{}'", s5);

    p_("------------ buf4 = move(test_buffer)");
    ipc_msgbuf2 buf4;
    buf4 = std::move(test_buffer);
    p_("test_buffer");
    test_buffer.dump();
    p_("buf4");
    buf4.dump();

    p_("------------- ::iovec");
    ::iovec *ivp = &buf3[0];
    for (int i = 0; i <= 5; i++, ivp++) {
	p_("ivp %{} %{}\nbuf %{} %{}",
	   ivp->iov_len, ivp->iov_base,
	   buf3[i].iov_len, buf3[i].iov_base);
    }

    p_("------------- vector");
    std::vector<int> vi;
    for (int i = 0; i < 10; i++) {
	vi.push_back(i);
    }
    buf3[2].iov_base = vi;
    buf3.dump();

    p_("------------- vector*");
    buf3[2].iov_base = nullptr;
    buf3[2].iov_base = &vi;
    buf3.dump();

    p_("------------- std::array");
    std::array<double, 5> dv {1.1, 2.2, 3.3, 4.4, 5.5};
    buf3[2].iov_base = dv;
    buf3.dump();

    p_("------------- std::array*");
    buf3[2].iov_base = nullptr;
    buf3[2].iov_base = &dv;
    buf3.dump();

    p_("------------- string");
    std::string s{"This is string"};
    buf3[2].iov_base = s;
    buf3.dump();

    p_("------------- string*");
    buf3[2].iov_base = &s;
    buf3.dump();

    p_("------------- nullptr");
    buf3[2].iov_base = nullptr;
    buf3.dump();

    p_("------------- float[4]");
    float dv2[4] = {1.1, 2.2, 3.3, 4.4,};
    buf3[2].iov_base = dv2;	// -> float*
    buf3.dump();

    p_("------------- float(*)[4]");
    buf3[2].iov_base = &dv2;
    buf3.dump();

    p_("-------------- get pointer with size check");
    {
	struct dummy_t {
	    char c[2];
	};
	std::array<dummy_t, 5>  dummydata;
	ipc_msgbuf buf;
	buf[1].iov_base = dummydata;

	dummy_t *dp;
	dummy_t (*dp5)[5];
	xy_t *xyp;
	size_t n;

	if (auto res = buf[1].strict_ptr(dp); res) {
	    p_("0: dp:%{}", dp);
	} else {
	    p_("0: dp:%{}, %{}", dp, res);
	}
	if (auto res = buf[1].strict_ptr(dp5); res) {
	    p_("1: dp:%{}", dp5);
	} else {
	    p_("1: dp:%{}, %{}", dp5, res);
	}
	if (auto res = buf[1].strict_ptr(dp, n); res) {
	    p_("2: dp:%{}, n:%{}", dp, n);
	} else {
	    p_("2: dp:%{}, %{}", dp, res);
	}

	if (auto res = buf[1].strict_ptr(xyp); res) {
	    p_("3: xyp:%{}", xyp);
	} else {
	    p_("3: xyp:%{}, %{}", xyp, res);
	}
	if (auto res = buf[1].relaxed_ptr(xyp); res) {
	    p_("4: xyp:%{}", xyp);
	} else {
	    p_("4: xyp:%{}, %{}", xyp, res);
	}

	const auto& robuf = buf;
	if (auto res = robuf[1].strict_ptr(dp, n); res) {
	    p_("6: dp:%{}, n:%{}", dp, n);
	} else {
	    p_("6: dp:%{}, %{}", dp, res);
	}
    }
}
