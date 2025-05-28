#include <variant>
#include <c7app.hpp>
#include <c7generator_r2.hpp>


using c7::p_;


struct inst_BEG {};
struct inst_END {};
struct inst_FIN {};

using inst_var = std::variant<inst_BEG, inst_END, inst_FIN>;


class EngineInterface {
public:
    EngineInterface(): gen_(1) {}
    virtual ~EngineInterface() {}
    virtual c7::result<> start() = 0;
    virtual void stop() = 0;

    c7::r2::generator<inst_var>::gen_iter begin() {
	return gen_.begin();
    }
    c7::r2::generator<inst_var>::gen_iter_end end() {
	return gen_.end();
    }

protected:
    using gen_output = c7::r2::generator<inst_var>::gen_output;
    void start(std::function<void(gen_output&)> func) {
	gen_.start(func);
    }

private:
    c7::r2::generator<inst_var> gen_;
};

class Engine: public EngineInterface {
public:
    struct parameter {};

    explicit Engine(parameter) {}

    c7::result<> start() override {
	// check paramter ...
	EngineInterface::start([this](auto& o){ generate(o); });
	return c7result_ok();
    }

    void stop() override {
	p_("stop");
	stop_ = true;
    }

private:
    bool stop_ = false;

    void generate(gen_output& out) {
	for (;;) {
	    out << inst_BEG{};
	    out << inst_END{};
	    if (stop_) {
		for (;;) {
		    out << inst_FIN{};
		}
	    }
	}
    }
};


class Driver {
public:
    explicit Driver(EngineInterface &engine): engine_(engine) {}

    c7::result<> start() {
	return engine_.start();
    }

    void loop(int n) {
	for (auto inst: engine_) {
	    if (!std::visit([this](auto inst){ return interpret(inst); }, inst)) {
		return;
	    }
	    if (n-- == 0) {
		engine_.stop();
	    }
	}
    }

private:
    EngineInterface &engine_;

    static bool interpret(inst_BEG) {
	p_("BEG");
	return true;
    }
    static bool interpret(inst_END) {
	p_("END");
	return true;
    }
    static bool interpret(inst_FIN) {
	p_("FIN");
	return false;
    }
};


int main()
{
    Engine concrete_engine{Engine::parameter{}};
    Driver driver(concrete_engine);
    if (auto res = driver.start(); !res) {
	c7exit(res);
    }
    driver.loop(4);
}
