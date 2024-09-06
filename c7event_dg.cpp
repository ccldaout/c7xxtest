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

class service_1: public service_interface<my_msgbuf> {
public:
    ~service_1() {
	p_("~service_1");
    }
    void on_message(monitor& mon, port_type& port, msgbuf_type& msg) override {
	p_("service_1::on_message()");
    }
};

class service_2: public service_interface<my_msgbuf> {
public:
    ~service_2() {
	p_("~service_2");
    }
    void on_message(monitor& mon, port_type& port, msgbuf_type& msg) override {
	p_("service_2::on_message()");
    }
};

class service_3: public service_interface<my_msgbuf> {
public:
    ~service_3() {
	p_("~service_3");
    }
    void on_message(monitor& mon, port_type& port, msgbuf_type& msg) override {
	p_("service_3::on_message()");
    }
};

class service_4: public service_interface<my_msgbuf> {
public:
    ~service_4() {
	p_("~service_4");
    }
    void on_message(monitor& mon, port_type& port, msgbuf_type& msg) override {
	p_("service_4::on_message()");
    }
};

class service_5: public service_interface<my_msgbuf> {
public:
    ~service_5() {
	p_("~service_5");
    }
    void on_message(monitor& mon, port_type& port, msgbuf_type& msg) override {
	p_("service_5::on_message()");
    }
};

class service_6: public service_interface<my_msgbuf> {
public:
    ~service_6() {
	p_("~service_6");
    }
    void on_message(monitor& mon, port_type& port, msgbuf_type& msg) override {
	p_("service_6::on_message()");
    }
};

class service_7: public service_interface<my_msgbuf> {
public:
    ~service_7() {
	p_("~service_7");
    }
    void on_message(monitor& mon, port_type& port, msgbuf_type& msg) override {
	p_("service_7::on_message()");
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

    p_("svc.on_message() ...");
    svc.on_message(mon, port, msg);
    p_("svc.on_message() ... end");

    p_("\nsvc2 = nullptr ...");
    svc2 = nullptr;
    p_("svc2 = nullptr ... end");

    p_("\nsvc.on_message() #2 ...");
    svc.on_message(mon, port, msg);
    p_("svc.on_message() #2 ... end");

    p_("\nexit");
}
