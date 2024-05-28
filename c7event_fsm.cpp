#include <c7event/ext/fsm.hpp>
#include <c7event/timer.hpp>
#include <c7event/msgbuf_impl.hpp>
#include <c7event/connector_impl.hpp>


using namespace c7::event;
using namespace c7::event::ext;


using c7::io_result;
typedef multipart_msgbuf<int, 3> my_msgbuf;


// state
enum {
    S_INIT,
    S_PREP,
    S_READY,
    S_FINAL,
    S_FAULT,
};

// event (trigger of state transition)
enum {
    EV_START,
    EV_UNIT1_READY,	// partial event
    EV_UNIT2_READY,	// partial event
    EV_READY,		// combined event
    EV_FINISH,
    EV_UNIT1_STOP,	// partial event
    EV_UNIT2_STOP,	// partial event
    EV_STOP,		// combined event
    EV_ERROR,
};

// callback ID on each transition
enum {
    CB_INIT_PREP,
    CB_PREP_READY,
    CB_PREP_FAULT,
    CB_FAULT_FINAL,
    CB_READY_FINAL,
    CB_FINAL_INIT,
};

// state transition table
static c7::fsm::driver<void>::transition trans[] = {
    { S_INIT, EV_START, S_PREP, CB_INIT_PREP },
    { S_PREP, EV_READY, S_READY,CB_PREP_READY },
    { S_PREP, EV_ERROR, S_FAULT, CB_PREP_FAULT },
    { S_FAULT, EV_FINISH, S_FINAL,CB_FAULT_FINAL },
    { S_READY, EV_FINISH, S_FINAL, CB_READY_FINAL },
    { S_FINAL, EV_STOP, S_INIT, CB_FINAL_INIT },
};


class test_service:
    public fsm_service<service_interface<my_msgbuf>> {
public:
    using base_type = fsm_service<service_interface<my_msgbuf>>;

    attach_id on_attached(monitor& mon, socket_port& port, provider_hint hint) override {
	auto sid = base_type::on_attached(mon, port, hint);
#define link_cb(n)	\
	ext_fsm.link_callback(CB_##n, [this](auto c, auto n){ cb_##n(c, n); })
	link_cb(INIT_PREP);
	link_cb(PREP_READY);
	link_cb(PREP_FAULT);
	link_cb(FAULT_FINAL);
	link_cb(READY_FINAL);
	link_cb(FINAL_INIT);
#undef link_cb
	ext_fsm.start();
	ext_fsm.commit(EV_START);
	c7echo("  put EV_START");
	return sid;
    }

    void on_message(monitor& mon, socket_port& port, my_msgbuf& mbuf) override {
    }

    void on_disconnected(monitor& mon, socket_port& port, io_result&) override {
    }

    void on_error(monitor& mon, socket_port& port, io_result&) override {
    }

private:
    using state_t = c7::fsm::driver<void>::state_t;

    void cb_INIT_PREP(state_t cur, state_t next) {
	c7echo("cb: INIT -> PREP");
	ext_fsm.commit(EV_UNIT1_READY);
	c7::event::timer_start(500*1000, 0, [this](auto, auto) {
#if 0
		ext_fsm.commit(EV_ERROR);
		c7echo("  put EV_ERROR");
#else
		ext_fsm.commit(EV_UNIT2_READY);
		c7echo("  put EV_UNIT2_READY");
#endif
	    });
	c7echo("cb: return");
    }

    void cb_PREP_READY(state_t cur, state_t next) {
	c7echo("cb: PREP -> READY");
	ext_fsm.commit(EV_FINISH);
	c7echo("  put EV_FINISH");
	c7echo("cb: return");
    }

    void cb_PREP_FAULT(state_t cur, state_t next) {
	c7echo("cb: PREP -> FAULT");
	ext_fsm.commit(EV_FINISH);
	c7echo("  put EV_FINISH");
	c7echo("cb: return");
    }

    void cb_FAULT_FINAL(state_t cur, state_t next) {
	c7echo("cb: FAULT -> FINAL");
	ext_fsm.commit(EV_UNIT1_STOP);
	c7echo("  put EV_UNIT1_STOP");
	c7::event::timer_start(500*1000, 0, [this](auto, auto) {
		ext_fsm.commit(EV_UNIT2_STOP);
	    });
	c7echo("cb: return");
    }

    void cb_READY_FINAL(state_t cur, state_t next) {
	c7echo("cb: READY -> FINAL");
	ext_fsm.commit(EV_UNIT2_STOP);
	c7echo("  put EV_UNIT2_STOP");
	c7::event::timer_start(500*1000, 0, [this](auto, auto) {
		ext_fsm.commit(EV_UNIT1_STOP);
		c7echo("  put EV_UNIT1_STOP");
	    });
	c7echo("cb: return");
    }

    void cb_FINAL_INIT(state_t cur, state_t next) {
	c7echo("cb: FINAL -> INIT");
	c7echo("cb: return");
	exit(0);
    }
};


int main()
{
    c7::fsm::driver<void> fsm;

    fsm.define_combined(EV_READY, {EV_UNIT1_READY, EV_UNIT2_READY});
    fsm.define_combined(EV_STOP,  {EV_UNIT1_STOP,  EV_UNIT2_STOP});
    fsm.add_transition(&trans);
    fsm.initial_state(S_INIT);

    if (auto res = fsm_provider::manage(std::move(fsm)); !res) {
	c7error(res);
    }

    const char *un_path = "../.test.socket";
    auto svr = c7::unix_server(un_path);

    auto sa = c7::sockaddr_unix(un_path).value();
    auto svc = std::make_shared<test_service>();
    if (auto res = c7::event::manage_connector(sa, std::move(svc)); !res) {
	c7error(res);
    }

    c7::event::forever();
}
