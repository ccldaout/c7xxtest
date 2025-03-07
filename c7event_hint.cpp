#include <c7event/service.hpp>
#include <c7app.hpp>

using c7::p_;

enum class hint_value: uint64_t { centerd, gemconf };

int main()
{
    c7::event::provider_hint h = hint_value::gemconf;
    
    p_("false? %{}", (h == hint_value::centerd));
    p_(" true? %{}", (h == hint_value::gemconf));
    p_(" true? %{}", (h != hint_value::centerd));
    p_("false? %{}", (h != hint_value::gemconf));

    const char *p = "abc";
    const char *q = "pqr";

    c7::event::provider_hint h2 = p;
    p_(" true? %{}",  (h2 == p));
    p_("false? %{}",  (h2 == q));
    p_("false? %{}",  (h2 != p));
    p_(" true? %{}",  (h2 != q));

    h2 = hint_value::gemconf;
    p_(" true? %{}",  (h == h2));
    p_("false? %{}",  (h != h2));
}
