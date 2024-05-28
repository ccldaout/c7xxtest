#include <c7app.hpp>
#include <c7event/ext/flagsync.hpp>
#include <c7event/msgbuf_impl.hpp>
#include <c7event/connector_impl.hpp>
#include <c7thread/event.hpp>


using namespace c7::event;


using c7::io_result;
typedef multipart_msgbuf<int, 3> my_msgbuf;


#define F_A	(1U<<0)
#define F_B	(1U<<1)
#define F_C	(1U<<2)
#define F_D	(1U<<3)
#define F_E	(1U<<4)
#define F_F	(1U<<5)


static c7::thread::event connected;


class test_service:
    public ext::flagsync_service<service_interface<my_msgbuf>> {
public:
    using base_type = ext::flagsync_service<service_interface<my_msgbuf>>;
    using flags_t = ext::flagsync_provider::flags_t;

    attach_id on_attached(monitor& mon, socket_port& port, provider_hint hint) override {
	c7echo("on_sttached ...");
	auto sid = base_type::on_attached(mon, port, hint);
	auto sp = shared_from_this();
	ext_flagsync.assign(F_A|F_B|F_C,              &test_service::on_ABC);
	ext_flagsync.assign(        F_C|F_D,          &test_service::on_CD);
	ext_flagsync.assign(F_A|F_B    |F_D,          &test_service::on_ABD);
	ext_flagsync.assign(F_A             |F_E|F_F, [this](flags_t& f){ on_AEF(f); });
	c7echo("on_sttached ... end");
	connected.set();
	return sid;
    }

    void on_message(monitor& mon, socket_port& port, my_msgbuf& mbuf) override {
    }

    void on_disconnected(monitor& mon, socket_port& port, io_result&) override {
    }

    void on_error(monitor& mon, socket_port& port, io_result&) override {
    }

private:
    void on_ABC(uint32_t& flags) {
	c7echo("on_ABC");
	flags |= F_B|F_C|F_D;
    }
    void on_CD(uint32_t& flags) {
	c7echo("on_CD");
	flags |= F_A;
	ext_flagsync.update(F_D, 0);
    }
    void on_ABD(uint32_t& flags) {
	c7echo("on_ABD");
	ext_flagsync.update(F_F, 0);
	flags |= F_A;
    }
    void on_AEF(uint32_t& flags) {
	c7echo("on_AEF");
	exit(0);
    }
};


int main()
{
    if (auto res = ext::flagsync_provider::manage(); !res) {
	c7error(res);
    }

    const char *un_path = "../.test.socket";
    auto svr = c7::unix_server(un_path);

    auto sa = c7::sockaddr_unix(un_path).value();
    auto svc = std::make_shared<test_service>();
    if (auto res = c7::event::manage_connector(sa, std::move(svc)); !res) {
	c7error(res);
    }

    c7::event::start_thread();
    connected.wait();

    if (auto res = c7::event::find<ext::flagsync_provider>(ext::flagsync_provider::manage_key); !res) {
	c7error(res);
    } else {
	auto prov = std::move(res.value());
	c7::sleep_ms(500);
	c7echo("prov->udpate#1 ...");
	prov->update(F_A|F_B|F_C, 0);
	c7echo("prov->udpate#2 ... end");
	c7::sleep_ms(500);
	prov->update(F_E, 0);
    }

    c7::event::wait_thread();
}
