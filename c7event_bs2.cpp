#include <c7app.hpp>
#include <c7socket.hpp>
#include <c7event/monitor.hpp>
#include <c7thread/thread.hpp>
#include <c7utils/movable_capture.hpp>


using c7::result;
using c7::event::provider_interface;
using c7::event::monitor;
using c7::io_result;


#define REC_SIZE_b	(4096)


static void
read_thread(c7::socket&& port)
{
    uint32_t buff[REC_SIZE_b/sizeof(uint32_t)];
    for (;;) {
	if (auto res = port.read_n(buff, REC_SIZE_b); !res) {
	    c7error("read_thread: %{}", res);
	} else {
	    c7echo("read_thread: %{}", buff[0]);
	}
    }
}


class my_acceptor: public provider_interface {
public:
    c7::delegate<void, c7::socket&, c7::result_base&> on_error;

    static auto make(c7::socket&& port) {
	auto p = new my_acceptor(std::move(port));
	return std::shared_ptr<my_acceptor>(p);
    }

    ~my_acceptor() override {}
    int fd() override;
    void on_manage(monitor& mon, int prvfd) override;
    void on_event(monitor& mon, int prvfd, uint32_t events) override;

private:
    c7::socket port_;

    explicit my_acceptor(c7::socket&& port);
};

my_acceptor::my_acceptor(c7::socket&& port):
    provider_interface(), port_(std::move(port))
{
}

int my_acceptor::fd()
{
    return port_;
}

void my_acceptor::on_manage(monitor& mon, int prvfd)
{
    port_.on_close += [&mon, prvfd](c7::fd&){ mon.unmanage(prvfd); };
}

void my_acceptor::on_event(monitor& mon, int, uint32_t)
{
    if (auto res = port_.accept(); !res) {
	on_error(port_, res);
    } else {
	c7::thread::thread th;
	auto sock = std::move(res.value());
	sock.on_close += [](c7::fd& fd) { c7echo("--- closed: %{}", fd); };
	th.target([ws=c7::movable_capture(sock)]() {
		read_thread(ws.unwrap());
	    });
	if (auto res = th.start(); !res) {
	    c7error("th.start failed: %{}", res);
	}
    }
}


static const char *un_path = "/usr/ebsys/tmp/c7event.socket";

int main(int argc, char **argv)
{
    if (argc != 2) {
	c7exit("Usage: %{} {s|c}", c7::app::progname);
    }

    if (*argv[1] == 's') {
	// server
	auto res = c7::unix_server(un_path);
	if (!res) {
	    c7error(res);
	}
	auto svr_sock = std::move(res.value());
	c7echo("svr_sock: %{}", svr_sock);

	auto acc = my_acceptor::make(std::move(svr_sock));
	c7::event::manage(std::move(acc));
	c7::event::forever();
    } else {
	// client
	c7::socket sock;
	if (auto res = c7::unix_client(un_path); !res) {
	    c7error("unix_client: %{}", res);
	} else {
	    sock = std::move(res.value());
	}
	for (int i = 0; i < 20; i++) {
	    static uint32_t buff[REC_SIZE_b / sizeof(uint32_t)];
	    buff[0] = i;
	    auto res = sock.write_n(&buff);
	    c7echo("write_n: %{}", res);
	    if (!res) {
		return 1;
	    }
	}
	return 0;
    }
}
