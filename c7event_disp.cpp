#include <c7app.hpp>
#include <c7event/msgbuf_impl.hpp>
#include <c7event/service.hpp>
#include <c7event/ext/delegate.hpp>
#include <c7event/ext/dispatcher.hpp>
#include <c7event/ext/forwarder.hpp>


using namespace c7::event;
using namespace c7::event::ext;


enum {
    EV_TEST0,
    EV_TEST1,
    EV_TEST2,
    EV_TEST3,
    EV_TEST4,
    EV_TEST5,
    EV_TEST6,
    EV_TEST7,
    EV_numof,
};


/*
 * test macro for testing setup_dispatcher.py
 */
#define MAC1
#define MAC2


using my_msgbuf = multipart_msgbuf<int32_t, 3>;

template <>
int32_t c7::event::get_event(const my_msgbuf& m) { return m.header; }


struct enter_hook {
    void enter_callback(monitor&, socket_port&, my_msgbuf& msg) {
	c7echo("enter_callback: 0x%{x}", msg.header);
    }
    void exit_callback(monitor&, socket_port&, my_msgbuf& msg) {
	c7echo("enter_hook: exit_callback: 0x%{x}", msg.header);
    }
    void show() {
	c7echo("enter_hook::show");
    }
};

struct enter_hook2 {
    void exit_callback(monitor&, socket_port&, my_msgbuf& msg) {
	c7echo("enter_hook2: exit_callback: 0x%{x}", msg.header);
    }
    void show() {
	c7echo("enter_hook2::show");
    }
};


// my_service

template <typename M>
class my_service: public delegate<forwarder<dispatcher<my_service<M>,
						       service_interface<my_msgbuf>,
						       enter_hook, enter_hook2
						       >>> {
public:

    void dispatcher_setup();

    void callback_default(monitor& mon, socket_port& port, const my_msgbuf& msg) override {
	c7echo("my: default: %{}", msg.header);
    }

#if defined(MAC1) && defined(MAC1)
    void callback_EV_TEST1(monitor& mon, socket_port& port, const my_msgbuf& msg) {
	c7echo("my: EV_TEST1: %{}", msg.header);
    }
#endif

    void callback_EV_TEST2(monitor& mon, socket_port& port, const my_msgbuf& msg) {
	c7echo("my: EV_TEST2: %{}", msg.header);
    }

    //[dispatcher:callback EV_TEST6 EV_TEST1:EV_TEST3]
    //[dispatcher:callback EV_TEST4]
    void callback_event_346(monitor& mon, socket_port& port, const my_msgbuf& msg) {
	c7echo("my: event_346: %{}", msg.header);
    }

    void on_disconnected(monitor& mon, socket_port& port, c7::io_result& io_res) override {
    }

    void on_error(monitor& mon, socket_port& port, c7::io_result& io_res) override {
    }
};

template <typename M>
void my_service<M>::dispatcher_setup()
{
    //[dispatcher:setup begin]
#if defined(MAC1) && defined(MAC1)
    this->dispatcher_set(EV_TEST1, &my_service::callback_EV_TEST1);
#endif
    this->dispatcher_set(EV_TEST2, &my_service::callback_EV_TEST2);
    this->dispatcher_set(EV_TEST6, &my_service::callback_event_346);
    this->dispatcher_set(EV_TEST1, EV_TEST3, &my_service::callback_event_346);
    this->dispatcher_set(EV_TEST4, &my_service::callback_event_346);
    //[dispatcher:setup end]
}


// outer_service

template <typename M>
class outer_service: public dispatcher<outer_service<M>, service_interface<my_msgbuf>> {
public:

    void dispatcher_setup();

    void callback_default(monitor& mon, socket_port& port, const my_msgbuf& msg) override {
	c7echo("outer: default: %{}", msg.header);
    }

    void callback_EV_TEST2(monitor& mon, socket_port& port, const my_msgbuf& msg) {
	c7echo("outer: EV_TEST2: %{}", msg.header);
    }

    //[dispatcher:callback EV_TEST3:EV_TEST5]
    void callback_event_345(monitor& mon, socket_port& port, const my_msgbuf& msg) {
	c7echo("outer: event_345: %{}", msg.header);
    }

    void on_disconnected(monitor& mon, socket_port& port, c7::io_result& io_res) override {
    }

    void on_error(monitor& mon, socket_port& port, c7::io_result& io_res) override {
    }
};

template <typename M>
void outer_service<M>::dispatcher_setup()
{
    //[dispatcher:setup begin]
    this->dispatcher_set(EV_TEST2, &outer_service::callback_EV_TEST2);
    this->dispatcher_set(EV_TEST3, EV_TEST5, &outer_service::callback_event_345);
    //[dispatcher:setup end]
}


// forwarder

static void callback(monitor& mon, socket_port& port, c7::io_result& io_res, const my_msgbuf& msg)
{
    c7echo("forward -> callback: %{}", msg.header);
}


int main()
{
    my_msgbuf msg;
    my_service<int> svc;
    auto sop = std::make_shared<outer_service<int>>();
    socket_port port;
    monitor mon;

    static_cast<enter_hook&>(svc).show();
    static_cast<enter_hook2&>(svc).show();

    auto fp = svc.ext_forwarder();
    fp->set_callback(callback);
    fp->subscribe({2});

    svc.ext_delegate += sop;

    service_interface<my_msgbuf> *sp = &svc;
    for (int i = 0; i < 8; i++) {
	msg.header = i;
	sp->on_message(mon, port, msg);
    }

    fp.reset();
    sop.reset();

    c7echo("---- reset forwarder proxy ----");
    for (int i = 0; i < 8; i++) {
	msg.header = i;
	sp->on_message(mon, port, msg);
    }
}
