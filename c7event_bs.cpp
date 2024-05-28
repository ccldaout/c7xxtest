#include <c7app.hpp>
#include <c7event/acceptor_impl.hpp>


using c7::result;
using c7::event::monitor;
using c7::event::provider_hint;
using c7::event::socket_port;
using c7::event::service_interface;
using c7::io_result;


#define REC_SIZE_b	(4096)


template <size_t RECSIZ>
class my_msgbuf {
public:
    // requirement: c7::event::receiver;
    template <typename Port> io_result recv(Port& port) {
	return port.read_n(buff_, RECSIZ);
    }

    // original
    template <typename T>
    T *as() {
	return reinterpret_cast<T*>(buff_);
    }

private:
    char buff_[RECSIZ];
};


template <size_t RECSIZ>
class my_service: public service_interface<my_msgbuf<RECSIZ>> {
public:
    using base_type = service_interface<my_msgbuf<RECSIZ>>;

    void on_message(monitor&, socket_port& port, my_msgbuf<RECSIZ>& msgbuf) override {
	auto up = msgbuf.template as<uint32_t>();
	c7echo("on_message: %{}", *up);
    }
    void on_disconnected(monitor&, socket_port& port, io_result& io_res) override {
	c7echo("on_disconnected: %{}", io_res);
	port.close();
    }
    void on_error(monitor&, socket_port& port, io_result& io_res) override {
	c7echo("on_error: %{}", io_res);
	port.close();
    }

private:
};


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

	std::string name = "server";
	c7::event::manage_acceptor(socket_port(std::move(svr_sock)),
				   std::make_shared<my_service<REC_SIZE_b>>());
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
