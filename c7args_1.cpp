#include <c7app.hpp>
#include <c7args.hpp>
#include <c7nseq/c_array.hpp>


using c7::p_;
using c7::P_;


namespace std {
void print_type(std::ostream& o, const std::string& sep, const vector<string>& sv);
}


class Parser: public c7::args::parser {
public:
    c7::result<> init() override {
	c7::result<> res;

	// bool
	{
	    c7::args::opt_desc d;
	    d.type = prm_type::BOOL;
	    d.opt_descrip = "bool test";
	    d.prm_name = "BOOL";
	    d.prm_descrip = "boolean value";

	    d.long_name = "verbose";
	    d.short_name = "v";
	    res << add_opt(d, &Parser::option_bool);

	    d.long_name = "verb23";
	    d.short_name = "";
	    d.prmc_min = 2;
	    d.prmc_max = 3;
	    res << add_opt(d, &Parser::option_bool);
	}

	// int
	{
	    c7::args::opt_desc d;
	    d.type = prm_type::INT;
	    d.opt_descrip = "integer test";
	    d.prm_name = "INT";
	    d.prm_descrip = "integer value";

	    d.long_name = "int";
	    d.prmc_min = 1;
	    d.prmc_max = 1;
	    res << add_opt(d, &Parser::option_int);

	    d.long_name = "int03";
	    d.prmc_min = 0;
	    d.prmc_max = 3;
	    res << add_opt(d, &Parser::option_int);
	}

	// uint
	{
	    c7::args::opt_desc d;
	    d.type = prm_type::UINT;
	    d.opt_descrip = "unsigned integer test";
	    d.prm_name = "UINT";
	    d.prm_descrip = "unsigned interger value";

	    d.long_name = "uint";
	    d.prmc_min = 1;
	    d.prmc_max = 1;
	    res << add_opt(d, &Parser::option_uint);

	    d.long_name = "uint03";
	    d.prmc_min = 0;
	    d.prmc_max = 3;
	    res << add_opt(d, &Parser::option_uint);
	}

	// real
	{
	    c7::args::opt_desc d;
	    d.type = prm_type::REAL;
	    d.opt_descrip = "real number test";
	    d.prm_name = "REAL";
	    d.prm_descrip = "real number value";

	    d.long_name = "real";
	    d.prmc_min = 1;
	    d.prmc_max = 1;
	    res << add_opt(d, &Parser::option_real);

	    d.long_name = "real03";
	    d.prmc_min = 0;
	    d.prmc_max = 3;
	    res << add_opt(d, &Parser::option_real);
	}

	// key
	{
	    c7::args::opt_desc d;
	    d.type = prm_type::KEY;
	    d.opt_descrip = "keys test";
	    d.prm_name = "KEY";
	    d.prm_descrip = "keyword value";
	    d.keys = c7::strvec{"big", "little", "unknown"};

	    d.long_name = "key";
	    d.prmc_min = 1;
	    d.prmc_max = 1;
	    d.ignore_case = false;
	    res << add_opt(d, &Parser::option_key);

	    d.long_name = "key03";
	    d.prmc_min = 0;
	    d.prmc_max = 3;
	    d.ignore_case = true;
	    res << add_opt(d, &Parser::option_key);
	}

	// reg
	{
	    c7::args::opt_desc d;
	    d.type = prm_type::REG;
	    d.opt_descrip = "regex test";
	    d.prm_name = "REG";
	    d.prm_descrip = "regex value";
	    d.keys = c7::strvec{
		R"((\d+)([a-z]+))",
		R"((id)|(number))",
		R"(([a-z_])(\w*))",
	    };

	    d.long_name = "reg";
	    d.prmc_min = 1;
	    d.prmc_max = 1;
	    d.ignore_case = false;
	    res << add_opt(d, &Parser::option_reg);

	    d.long_name = "reg03";
	    d.prmc_min = 0;
	    d.prmc_max = 10;
	    d.ignore_case = true;
	    res << add_opt(d, &Parser::option_reg);
	}

	// date
	{
	    c7::args::opt_desc d;
	    d.type = prm_type::DATE;
	    d.opt_descrip = "date test";
	    d.prm_name = "DATE";
	    d.prm_descrip = "date value";

	    d.long_name = "date";
	    d.prmc_min = 1;
	    d.prmc_max = 1;
	    res << add_opt(d, &Parser::option_date);

	    d.long_name = "date03";
	    d.prmc_min = 0;
	    d.prmc_max = 3;
	    res << add_opt(d, &Parser::option_date);
	}

	// any
	{
	    c7::args::opt_desc d;
	    d.type = prm_type::ANY;
	    d.opt_descrip = "any test";
	    d.prm_name = "ANY";
	    d.prm_descrip = "any value";

	    d.long_name = "any";
	    d.prmc_min = 1;
	    d.prmc_max = 1;
	    res << add_opt(d, &Parser::option_any);

	    d.long_name = "any03";
	    d.prmc_min = 0;
	    d.prmc_max = 3;
	    res << add_opt(d, &Parser::option_any);
	}

	return res;
    }

    c7::result<> option_bool(const c7::args::opt_desc& desc,
			     const std::vector<c7::args::opt_value>& values) {
	p_("option_bool: %{}", desc);
	for (auto& v: values) {
	    p_("           : %{}", v);
	}
	return c7result_ok();
    }

    c7::result<> option_int(const c7::args::opt_desc& desc,
			    const std::vector<c7::args::opt_value>& values) {
	p_("option_int: %{}", desc);
	for (auto& v: values) {
	    p_("           : %{}", v);
	}
	return c7result_ok();
    }

    c7::result<> option_uint(const c7::args::opt_desc& desc,
			     const std::vector<c7::args::opt_value>& values) {
	p_("option_uint: %{}", desc);
	for (auto& v: values) {
	    p_("           : %{}", v);
	}
	return c7result_ok();
    }

    c7::result<> option_real(const c7::args::opt_desc& desc,
			     const std::vector<c7::args::opt_value>& values) {
	p_("option_real: %{}", desc);
	for (auto& v: values) {
	    p_("           : %{}", v);
	}
	return c7result_ok();
    }

    c7::result<> option_key(const c7::args::opt_desc& desc,
			    const std::vector<c7::args::opt_value>& values) {
	p_("option_key: %{}", desc);
	for (auto& v: values) {
	    p_("           : %{}", v);
	}
	return c7result_ok();
    }

    c7::result<> option_reg(const c7::args::opt_desc& desc,
			    const std::vector<c7::args::opt_value>& values) {
	p_("option_reg: %{}", desc);
	for (auto& v: values) {
	    p_("           : %{}", v);
	}
	return c7result_ok();
    }

    c7::result<> option_date(const c7::args::opt_desc& desc,
			     const std::vector<c7::args::opt_value>& values) {
	p_("option_date: %{}", desc);
	for (auto& v: values) {
	    p_("           : %{}", v);
	}
	return c7result_ok();
    }

    c7::result<> option_any(const c7::args::opt_desc& desc,
			    const std::vector<c7::args::opt_value>& values) {
	p_("option_any: %{}", desc);
	for (auto& v: values) {
	    p_("           : %{}", v);
	}
	return c7result_ok();
    }

private:
    void show_value(const c7::args::opt_desc& desc,
		    const std::vector<c7::args::opt_value>& values) {
	
    }
};


static void test(Parser& parser, c7::strvec&& sv)
{
    p_("-----------------------------------------------------------\n"
       "test ... %{}", c7::str::join(" ", sv));

    auto pv = sv.c_strv();
    char **argv = pv.data();

    if (auto res = parser.parse(argv); !res) {
	c7echo(res);
    } else {
	argv = res.value();
	p_("\t%{}", c7::str::join(" ", const_cast<const char**>(argv)));
    }
}


int main(int argc, char **argv)
{
    Parser parser;
    if (auto res = parser.init(); !res) {
	c7exit(res);
    }

    test(parser, {"--verbose", "-v", "--verb23=y,True,0", "-a"});
    test(parser, {"--int", "5", "--int03", "--int03=1,2", "--int=9"});
    test(parser, {"--uint", "5", "--uint03", "--uint03=1,2", "--uint=9"});
    test(parser, {"--real", "5.1", "--real03", "--real03=1.1,2,3.14", "--real=9.999"});
    test(parser, {"--key", "little", "--key03", "--key03=little,BIG,Unknown", "--key=BIG"});
    test(parser, {"--reg", "_1abc", "--reg03", "--reg03=123abc,number,unknown,A2", "--reg=B2"});
    test(parser, {"--date", "1133", "--date03", "--date03=1133.29,2412241948", "--date=09240830.51"});
    test(parser, {"--any", "ab+*a,b", "--any03", "--any03=ab+*a,bbb,=a/b", "--any==,="});

    c7::strvec usage;
    parser.append_usage(usage, 8, 24);
    p_("-----------------------------------------------------------\n"
       "Usage: %{} [options ...]\n"
       "  [option]\n"
       "%{\n}", c7::app::progname, usage);
}
