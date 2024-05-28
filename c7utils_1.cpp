#include <c7app.hpp>
#include <c7utils.hpp>


using namespace std;

using c7::p_;


static void pwd_test()
{
    std::vector<std::string> users{"ebsys", "noname"};
    for (auto user: users) {
	auto r = c7::passwd::by_name(user);
	if (r) {
	    auto& pwd = r.value();
	    p_("%{} -> %{}, %{}, %{}", user, pwd->pw_uid, pwd->pw_dir, pwd->pw_shell);
	} else {
	    p_("by_name error: %{}", r);
	}
    }

    std::vector<::uid_t> uids{1204, 1200};
    for (auto uid: uids) {
	auto r = c7::passwd::by_uid(uid);
	if (r) {
	    auto& pwd = r.value();
	    p_("%{} -> %{}, %{}, %{}", uid, pwd->pw_name, pwd->pw_dir, pwd->pw_shell);
	} else {
	    p_("by_uid error: %{}", r);
	}
    }
}


struct data_t {
    int data[10];
    typedef c7::c_array_iterator<int> iterator;
    typedef c7::c_array_iterator<const int> const_iterator;

    data_t() {
	for (int i = 0; i < c7_numberof(data); i++) {
	    data[i] = i;
	}
    }

    iterator begin() {
	return iterator(data, 0);
    }
    iterator end() {
	return iterator(data, c7_numberof(data));
    }
    const_iterator begin() const {
	p_("const_iterator");
	return const_iterator(data, 0);
    }
    const_iterator end() const {
	return const_iterator(data, c7_numberof(data));
    }
};

static void c_array_test()
{
    data_t data;
    for (auto& d: data) {
	p_("%{} ", d);
    }
    p_("");

    p_("--");

    const data_t& cdata = data;
    for (auto& d: cdata) {
	p_("%{} ", d);
    }
    p_("");
}

static void storage_test()
{
    c7::storage st{8192};
    int n = 10000;
    if (auto res = st.reserve(sizeof(int)*n); !res) {
	c7error(res);
    }
    st.clear();
    int *p = st;
    for (int i = 0; i < n; i++, p++) {
	*p = i;
    }
    auto st2 = st.copy_to();
    c7echo("st : size:%{}, addr:%{}", st.size(), st.addr());
    c7echo("st2: size:%{}, addr:%{}", st2.size(), st2.addr());
    p = st2;
    for (int i = 0; i < n; i++, p++) {
	if (*p != i) {
	    c7error("*p:%{} != i:%{}", *p, i);
	}
    }

    auto st3 = std::move(st);
    c7echo("st : size:%{}, addr:%{}", st.size(), st.addr());
    c7echo("st3: size:%{}, addr:%{}", st3.size(), st3.addr());

    if (auto res = st3.reserve(sizeof(int)*(n * 3)); !res) {
	c7error(res);
    }

    c7echo("st3: size:%{}, addr:%{}", st3.size(), st3.addr());
    p = st3;
    for (int i = 0; i < n; i++, p++) {
	if (*p != i) {
	    c7error("*p:%{} != i:%{}", *p, i);
	}
    }
}


int main()
{
    pwd_test();
    c_array_test();
    storage_test();
    return 0;
}
