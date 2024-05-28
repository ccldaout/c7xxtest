#include <unistd.h>
#include <c7app.hpp>
#include <c7event/acceptor_impl.hpp>
#include <c7event/connector_impl.hpp>
#include <c7event/msgbuf_impl.hpp>
#include <c7event/shared_port.hpp>
#include <c7event/ext/attached.hpp>


using c7::p_;

struct my_header {
    int32_t event;
    void print(std::ostream& o, const std::string&) const {
	c7::format(o, "%{}", event);
    }
};

using my_msgbuf = c7::event::multipart_msgbuf<my_header, 5>;

using c7::result;
using c7::event::monitor;
using c7::event::provider_hint;
using c7::event::shared_port;
using c7::event::service_interface;
using c7::io_result;

namespace ext = c7::event::ext;


#define UNIX_DOMAIN


class svr_service: public ext::attached<service_interface<my_msgbuf, shared_port>> {
public:
    using base_type = service_interface<my_msgbuf, shared_port>;
    svr_service(const std::string& name): name_(name) {}

    attach_id on_attached(monitor& mon, port_type& port, provider_hint hint) override {
	auto sid = base_type::on_attached(mon, port, hint);

	if (auto res = port->peer(); res) {
	    auto& peer = res.value();
	    p_("peer: %{}", peer);
	}

	p_("on_attached: %{}", port);
	ext_attached.add(port);

	port.tcp_nodelay(true);

	auto sp = shared_from_this();
	p_("this: %{}\nsp  : %{}", this, sp.get());
	p_("my name: %{}", static_cast<svr_service*>(sp.get())->name_);

	my_msgbuf msgbuf;
	msgbuf.header.event = getpid();
	msgbuf[1].iov_base = name_;

	c7echo("send %{} ports", ext_attached.size());
	if (auto res = msgbuf.send(ext_attached); !res) {
	    c7echo(res);
	    for (auto& [p, io_res]: ext_attached.errors()) {
		c7echo(io_res.get_result());
		p->close();
	    }
	}

	return sid;
    }

    void on_message(monitor&, port_type& port, my_msgbuf& msgbuf) override {
	p_("on_message: %{}, event:%{}, <%{}>",
	   port,
	   msgbuf.header.event,
	   msgbuf[1].iov_base.template as<char>());

	c7echo("forward %{} ports", ext_attached.size());
	if (auto res = msgbuf.send(ext_attached); !res) {
	    c7echo(res);
	    for (auto& [p, io_res]: ext_attached.errors()) {
		c7echo(io_res.get_result());
		p->close();
	    }
	}
    }

    void on_disconnected(monitor&, port_type& port, io_result& io_res) override {
	p_("on_disconnected: %{}, %{}", port, io_res);
	port.close();
    }

    void on_error(monitor&, port_type& port, io_result& io_res) override {
	p_("on_error: %{}, %{}", port, io_res);
	port.close();
    }

    detach_id on_detached(monitor& mon, port_type& port, provider_hint hint) override {
	p_("on_detached: %{}", port);
	return base_type::on_detached(mon, port, hint);
    }

private:
    std::string name_;
};


class cli_service: public service_interface<my_msgbuf, shared_port> {
public:
    using base_type = service_interface<my_msgbuf, shared_port>;
    cli_service(const std::string& name): name_(name) {}

    attach_id on_attached(monitor& mon, port_type& port, provider_hint hint) override {
	auto sid = base_type::on_attached(mon, port, hint);

	p_("on_attached: %{}", port);
	port.tcp_nodelay(true);

	my_msgbuf msgbuf;
	msgbuf.header.event = getpid();
	msgbuf[1].iov_base = name_;

	if (auto res = msgbuf.send(port); !res) {
	    c7echo(res.get_result());
	    port.close();
	}
	return sid;
    }

    void on_message(monitor&, port_type& port, my_msgbuf& msgbuf) override {
	p_("on_message: %{}, event:%{}, <%{}>",
	   port,
	   msgbuf.header.event,
	   msgbuf[1].iov_base.template as<char>());
    }

    void on_disconnected(monitor&, port_type& port, io_result& io_res) override {
	p_("on_disconnected: %{}, %{}", port, io_res);
	port.close();
    }

    void on_error(monitor&, port_type& port, io_result& io_res) override {
	p_("on_error: %{}, %{}", port, io_res);
	port.close();
    }

    detach_id on_detached(monitor& mon, port_type& port, provider_hint hint) override {
	p_("on_detached: %{}", port);
	return base_type::on_detached(mon, port, hint);
    }

private:
    std::string name_;
};


static const char *un_path = "/usr/ebsys/tmp/c7event.socket";

int main(int argc, char **argv)
{
    if (argc != 2) {
	c7exit("Usage: %{} {s|c}", c7::app::progname);
    }

    if (*argv[1] == 's') {
	// server
#if defined(UNIX_DOMAIN)
	auto res = c7::unix_server(un_path);
#else
	auto res = c7::tcp_server("localhost", 21222);
#endif
	if (!res) {
	    c7error(res);
	}

	auto svr_sock = std::move(res.value());
	c7echo("svr_sock: %{}", svr_sock);

	c7::event::manage_acceptor(shared_port(std::move(svr_sock)),
				   std::make_shared<svr_service>("server"));
    } else {
	// client
	std::string name = c7::format("client<%{}>", getpid());
#if defined(UNIX_DOMAIN)
	auto res = c7::sockaddr_unix(un_path);
#else
	auto res = c7::sockaddr_ipv4("localhost", 21222);
#endif
	c7::event::manage_connector(res.value(),
				    std::make_shared<cli_service>(name));
    }

    c7::event::forever();
}
