#include <c7socket.hpp>
#include <c7app.hpp>
#include <c7utils.hpp>
#include <c7thread.hpp>
#include <c7signal.hpp>

static char buffer[512*1024*1024];

static void close_sock(c7::socket& sock)
{
    c7::sleep_ms(2000);
    c7echo("sock.shutdown");
    sock.shutdown_rw();
}

int main(int argc, char **argv)
{
    c7::signal::handle(SIGPIPE, SIG_IGN);

    c7::socket sock;
    if (auto res = c7::tcp_client(argv[1], 2000); !res) {
	c7exit(res);
    } else {
	sock = std::move(res.value());
    }
    c7::thread::thread th;
    th.target([&sock](){ close_sock(sock);});
    th.start();

    for (int i = 0; i < 32768; i++) {
	c7echo("sock.write_n ...");
	if (auto res = sock.write_n(&buffer); !res) {
	    c7echo("sock.write_n -> %{}", res);
	    break;
	}
    }
    c7echo("main exit");
}
