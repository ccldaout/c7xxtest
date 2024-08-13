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
    ~service_1() {
	p_("~service_1");
    }
    void on_message(monitor& mon, port_type& port, msgbuf_type& msg) override {
	p_("service_1");
    }
};

class service_2: public delegate<service_interface<my_msgbuf>> {
public:
    ~service_2() {
	p_("~service_2");
    }
    void on_message(monitor& mon, port_type& port, msgbuf_type& msg) override {
	p_("service_2");
    }
};

class service_3: public delegate<service_interface<my_msgbuf>> {
public:
    ~service_3() {
	p_("~service_3");
    }
    void on_message(monitor& mon, port_type& port, msgbuf_type& msg) override {
	p_("service_3");
    }
};

class service_4: public delegate<service_interface<my_msgbuf>> {
public:
    ~service_4() {
	p_("~service_4");
    }
    void on_message(monitor& mon, port_type& port, msgbuf_type& msg) override {
	p_("service_4");
    }
};

class service_5: public delegate<service_interface<my_msgbuf>> {
public:
    ~service_5() {
	p_("~service_5");
    }
    void on_message(monitor& mon, port_type& port, msgbuf_type& msg) override {
	p_("service_5");
    }
};

class service_6: public delegate<service_interface<my_msgbuf>> {
public:
    ~service_6() {
	p_("~service_6");
    }
    void on_message(monitor& mon, port_type& port, msgbuf_type& msg) override {
	p_("service_6");
    }
};

class service_7: public delegate<service_interface<my_msgbuf>> {
public:
    ~service_7() {
	p_("~service_7");
    }
    void on_message(monitor& mon, port_type& port, msgbuf_type& msg) override {
	p_("service_7");
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
    auto svc5 = std::make_shared<service_5>();
    auto svc6 = std::make_shared<service_6>();
    auto svc7 = std::make_shared<service_7>();

    socket_port port;
    monitor mon;

    svc.ext_delegate += svc3;
    svc.ext_delegate.install( 10, svc1);
    svc.ext_delegate.install(-10, svc6);
    svc.ext_delegate += svc4;
    svc.ext_delegate.install( 10, svc2);
    svc.ext_delegate += svc5;
    svc.ext_delegate.install(-10, svc7);

    svc.on_message(mon, port, msg);

    svc2 = nullptr;

    svc.on_message(mon, port, msg);

    p_("exit");
}
