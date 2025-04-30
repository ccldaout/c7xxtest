#include <c7format.hpp>
#include <c7socket.hpp>
#include <c7thread.hpp>
#include <c7utils/time.hpp>


#define PORT	22000
using c7::p_;


static void server(c7::socket& svr)
{
    p_("server: %{}", svr);
    auto res2 = svr.accept();
    if (!res2) {
	p_("%{}", res2);
	return;
    }
    auto sock = std::move(res2.value());

    auto defer = c7::defer([&](){ sock.close(); });

    if (auto res = sock.write("data", 5); !res) {
	p_("%{}", res);
	return;
    }

    c7::sleep_us(1000000);

    if (auto res = sock.write("end", 4); !res) {
	p_("%{}", res);
	return;
    }
}


static void test1()
{
    auto res = c7::tcp_server("", PORT);
    if (!res) {
	p_("%{}", res);
	return;
    }
    auto svr = std::move(res.value());

    c7::thread::thread th;
    th.target(server, std::ref(svr));
    th.start();

    res = c7::tcp_client("localhost", PORT);
    if (!res) {
	p_("%{}", res);
	return;
    }
    auto sock = std::move(res.value());

    char buff[512];

    c7::sleep_us(1000);
    sock.set_nonblocking(true);
    auto r = sock.read_n(buff, 10);
    p_("%{} <%{}>", r, buff);
    buff[0] = 0;

    sock.set_nonblocking(true);
    r = sock.read_n(buff, 10);
    p_("%{} <%{}>", r, buff);
    buff[0] = 0;

    sock.set_nonblocking(false);
    r = sock.read_n(buff, 10);
    p_("%{} <%{}>", r, buff);
    buff[0] = 0;

    r = sock.read_n(buff, 10);
    p_("%{} <%{}>", r, buff);
    buff[0] = 0;

    sock.close();
}



int main()
{
    test1();
    return 0;
}
