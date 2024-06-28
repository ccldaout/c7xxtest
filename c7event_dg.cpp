#include <c7app.hpp>
#include <c7event/msgbuf_impl.hpp>
#include <c7event/ext/delegate.hpp>

using c7::p_;
using namespace c7::event;
using namespace c7::event::ext;


using my_msgbuf = multipart_msgbuf<int32_t, 3>;

template <>
int32_t c7::event::get_event(const my_msgbuf& m) { return m.header; }


class main_service: public delegate<service_interface<my_msgbuf>> {
};

class service_1: public delegate<service_interface<my_msgbuf>> {
public:
    void on_message(monitor& mon, port_type& port, msgbuf_type& msg) override {
	p_("service_1");
    }
};

class service_2: public delegate<service_interface<my_msgbuf>> {
public:
    void on_message(monitor& mon, port_type& port, msgbuf_type& msg) override {
	p_("service_2");
    }
};

class service_3: public delegate<service_interface<my_msgbuf>> {
public:
    void on_message(monitor& mon, port_type& port, msgbuf_type& msg) override {
	p_("service_3");
    }
};

class service_4: public delegate<service_interface<my_msgbuf>> {
public:
    void on_message(monitor& mon, port_type& port, msgbuf_type& msg) override {
	p_("service_4");
    }
};


int main()
{
    my_msgbuf msg;
    main_service svc;

    auto svc1 = std::make_shared<service_1>();
    auto svc2 = std::make_shared<service_2>();
    auto svc3 = std::make_shared<service_3>();
    auto svc4 = std::make_shared<service_4>();

    socket_port port;
    monitor mon;

    svc.ext_delegate += svc2;
    svc.ext_delegate.install(10, svc1);
    svc.ext_delegate.install(-10, svc4);
    svc.ext_delegate += svc3;

    svc.on_message(mon, port, msg);
}
