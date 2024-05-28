#include <c7app.hpp>
#include <c7fsm.hpp>
#include <c7event/monitor.hpp>

enum {
    S_INIT,
    S_PREP,
    S_READY,
    S_FINAL,
    S_FAULT,
};

enum {
    EV_START,
    EV_UNIT1_READY,
    EV_UNIT2_READY,
    EV_READY,
    EV_FINISH,
    EV_UNIT1_STOP,
    EV_UNIT2_STOP,
    EV_STOP,
    EV_ERROR,
};

int main()
{
    c7::fsm::driver<c7::fsm::machine> drv;

    drv.define_combined(EV_READY, {EV_UNIT1_READY, EV_UNIT2_READY});
    drv.define_combined(EV_STOP,  {EV_UNIT1_STOP,  EV_UNIT2_STOP});

    drv.add_transition(S_INIT, EV_START, S_PREP,
		       [](auto& fsm, auto, auto) {
			   c7echo("> S_PREP");
			   fsm.commit(EV_UNIT1_READY);
#if 0
			   fsm.commit(EV_ERROR);
#else
			   fsm.commit(EV_UNIT2_READY);
#endif
		       });

    drv.add_transition(S_PREP, EV_READY, S_READY,
		       [](auto& fsm, auto, auto) {
			   c7echo("> S_READY");
			   fsm.commit(EV_FINISH);
		       });

    drv.add_transition(S_PREP, EV_ERROR, S_FAULT,
		       [](auto& fsm, auto, auto) {
			   c7echo("> S_FAULT");
			   fsm.commit(EV_FINISH);
		       });

    drv.add_transition(S_FAULT, EV_FINISH, S_FINAL,
		       [](auto& fsm, auto, auto) {
			   c7echo("> S_FINAL");
			   fsm.commit(EV_UNIT1_STOP);
			   fsm.commit(EV_UNIT2_STOP);
		       });

    drv.add_transition(S_READY, EV_FINISH, S_FINAL,
		       [](auto& fsm, auto, auto) {
			   c7echo("> S_FINAL");
			   fsm.commit(EV_UNIT2_STOP);
			   fsm.commit(EV_UNIT1_STOP);
		       });

    drv.add_transition(S_FINAL, EV_STOP, S_INIT,
		       [](auto& fsm, auto, auto) {
			   c7echo("> S_INIT");
			   exit(0);
		       });

    drv.initial_state(S_INIT);
    drv.dump();

    c7::fsm::machine fsm(std::move(drv));
    fsm.loop(EV_START, {S_INIT}, fsm);
}
