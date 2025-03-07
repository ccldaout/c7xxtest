#include <c7app.hpp>
#include <c7args.hpp>


using c7::p_;
using c7::P_;


struct Config {
    struct Ident {
	std::string reserved;
	std::string others;
	c7::strvec match;
    };

    bool verbose;
    int level;
    c7::strvec locales;
    std::vector<Ident> idents;
    time_t date;
};

class ArgParser: public c7::args::parser {
public:
    explicit ArgParser(Config& c): conf_(c) {}
    c7::result<> init() override;
    callback_t opt_verbose;
    callback_t opt_level;
    callback_t opt_locale;
    callback_t opt_id;
    callback_t opt_date;
    callback_t opt_help;

private:
    Config& conf_;

    void dump(const char *func,
	      const opt_desc& desc,
	      const std::vector<opt_value>& vals) {
	if (conf_.verbose) {
	    p_("\n*** %{}: %{}", func, desc);
	    for (auto& v: vals) {
		p_("%{}", v);
	    }
	}
    }
};


c7::result<>
ArgParser::init()
{
    c7::result<> res;

    // verbose
    {
	c7::args::opt_desc d;
	d.long_name = "verbose";
	d.short_name = "v";
	d.opt_descrip = "verbose";
	d.type = prm_type::BOOL;
	d.prmc_min = 0;
	d.prmc_max = 1;
	res << add_opt(d, &ArgParser::opt_verbose);
    }

    // level
    {
	c7::args::opt_desc d;
	d.long_name = "level";
	d.short_name = "l";
	    d.opt_descrip = "level";
	d.type = prm_type::INT;
	d.prm_name = "LEVEL";
	d.prm_descrip = "integer value";
	d.prmc_min = 1;
	d.prmc_max = 1;
	res << add_opt(d, &ArgParser::opt_level);
    }

    // locale
    {
	c7::args::opt_desc d;
	d.long_name = "locale";
	d.short_name = "L";
	    d.opt_descrip = "locale";
	d.type = prm_type::KEY;
	d.prm_name = "LOCALE";
	d.prm_descrip = "locale name";
	d.keys = c7::strvec{"C", "ja_JP", "en_US"};
	d.prmc_min = 0;
	d.prmc_max = d.keys.size();
	res << add_opt(d, &ArgParser::opt_locale);
    }

    // id
    {
	c7::args::opt_desc d;
	d.long_name = "id";
	d.prmc_min = 1;
	d.opt_descrip = "identifier";
	d.type = prm_type::REG;
	d.prm_name = "ID";
	d.prm_descrip = "reserved or generic identifier";
	d.keys = c7::strvec{
	    "(int)|(long)|(float)|(double)",
	    R"--(([[:alpha:]_])(\w*))--",
	};
	d.prmc_min = 1;
	d.prmc_max = -1U;
	res << add_opt(d, &ArgParser::opt_id);
    }

    // date
    {
	c7::args::opt_desc d;
	d.long_name = "date";
	d.type = prm_type::DATE;
	d.opt_descrip = "date time";
	d.prm_name = "DATE";
	d.prm_descrip = "date value";
	d.prmc_min = 1;
	d.prmc_max = 1;
	res << add_opt(d, &ArgParser::opt_date);
    }

    // help
    {
	c7::args::opt_desc d;
	d.long_name = "help";
	d.short_name = "h";
	d.opt_descrip = "this help";
	res << add_opt(d, &ArgParser::opt_help);
    }
    return res;
}


c7::result<>
ArgParser::opt_verbose(const opt_desc& desc,
		       const std::vector<opt_value>& vals)
{
    conf_.verbose = (vals.empty() || vals[1].b);
    dump(__func__, desc, vals);
    return c7result_ok();
}

c7::result<>
ArgParser::opt_level(const opt_desc& desc,
		     const std::vector<opt_value>& vals)
{
    dump(__func__, desc, vals);
    conf_.level = vals[0].i;
    return c7result_ok();
}

c7::result<>
ArgParser::opt_locale(const opt_desc& desc,
		      const std::vector<opt_value>& vals)
{
    dump(__func__, desc, vals);
    for (auto& v: vals) {
	conf_.locales.push_back(vals[0].param);
    }
    return c7result_ok();
}

c7::result<>
ArgParser::opt_id(const opt_desc& desc,
		  const std::vector<opt_value>& vals)
{
    dump(__func__, desc, vals);
    for (auto& v: vals) {
	Config::Ident id{};
	if (v.key_index == 0) {
	    id.reserved = v.param;
	} else {
	    id.others = v.param;
	}
	id.match = v.regmatch;
	conf_.idents.push_back(std::move(id));
    }
    return c7result_ok();
}

c7::result<>
ArgParser::opt_date(const opt_desc& desc,
		    const std::vector<opt_value>& vals)
{
    dump(__func__, desc, vals);
    conf_.date = vals[1].t;
    return c7result_ok();
}

c7::result<>
ArgParser::opt_help(const opt_desc& desc,
		    const std::vector<opt_value>& vals)
{
    dump(__func__, desc, vals);

    c7::strvec usage;
    usage += c7::format("Usage: %{} [options ...]"
			"  option",
			c7::app::progname);
    append_usage(usage, 4, 20);
    for (auto& s: usage) {
	std::cout << s << std::endl;
    }

    return c7result_ok();
}


int main(int argc, char **argv)
{
    Config cfg;
    ArgParser parser{cfg};

    if (auto res = parser.init(); !res) {
	c7error(res);
    }
    if (auto res = parser.parse(argv); !res) {
	c7error(res);
    } else {
	argv = res.value();
    }
    
    // print Config
    p_("\nConfig:");
    p_("  verbose: %{}", cfg.verbose);
    p_("  level: %{}", cfg.level);
    for (auto& s: cfg.locales) {
	p_("  locales: %{}", s);
    }
    for (auto& id: cfg.idents) {
	P_("  id: reserved:%{}, others:%{}, match:{",
	   id.reserved, id.others);
	for (auto& s: id.match) {
	    P_(" <%{}>", s);
	}
	p_(" }");
    }
    p_("  date:%{T%Y %m/%d %H:%M:%S}", cfg.date);

    // args
    p_("argv:");
    for (; *argv; argv++) {
	p_("  <%{}>", *argv);
    }
}
