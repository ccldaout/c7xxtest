#include <functional>		// std::hash
#include <tuple>
#include <unordered_map>
#include <c7app.hpp>
#include <c7hash/pair.hpp>


using c7::p_;


enum location_t {
    STAGE,
    ALC,
};


using actmap_key_t = std::pair<location_t, location_t>;


class director {
public:
    director() { init(); }
    void call(location_t b, location_t e);

private:
    using callback_t   = void (director::*)();
    
    std::unordered_map<actmap_key_t, callback_t> actmap_;

    void init();
    void func1() { p_("func1"); }
    void func2() { p_("func2"); }
};

void
director::init()
{
    actmap_.emplace(actmap_key_t{STAGE, ALC}, &director::func1);
    actmap_.emplace(actmap_key_t{ALC, STAGE}, &director::func2);
}

void
director::call(location_t b, location_t e)
{
    if (auto it = actmap_.find(actmap_key_t{b, e}); it != actmap_.end()) {
	(this->*(*it).second)();
    } else {
	p_("not found");
    }
}

int main()
{
    director d;
    d.call(ALC, STAGE);
    d.call(STAGE, STAGE);
    d.call(STAGE, ALC);
}
