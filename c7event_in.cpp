#include <c7app.hpp>
#include <c7nseq/reverse.hpp>
#include <c7thread/thread.hpp>
#include <c7utils.hpp>
#include <c7event/inotify.hpp>


using c7::p_;


static std::shared_ptr<c7::event::inotify_provider> in_provider;


class inotify_dirs {
public:
    void add_route(int trg_wd, const std::string& trg_name) {
	if (wdv_.size() <= static_cast<size_t>(trg_wd)) {
	    wdv_.resize(trg_wd + 1);
	}
	wdv_[trg_wd].parent_wd = -1;
	wdv_[trg_wd].name      = trg_name;
    }

    void add_dir(int parent_wd, int trg_wd, const std::string& trg_name) {
	if (wdv_.size() <= static_cast<size_t>(trg_wd)) {
	    wdv_.resize(trg_wd + 1);
	}
	wdv_[trg_wd].parent_wd = parent_wd;
	wdv_[trg_wd].name      = trg_name;
    }

    void rm_dir(int trg_wd) {
	auto& wd = wdv_[trg_wd];
	wd.parent_wd = -1;
	wd.name.clear();
    }

    void moved_from(int from_wd, uint32_t cookie, const std::string& from_name) {
	moving_dir md;
	md.from_wd = from_wd;
	md.from_name = from_name;
	mvdic_.insert_or_assign(cookie, md);
    }

    bool moved_to(int to_wd, uint32_t cookie, const std::string& to_name) {
	if (auto it = mvdic_.find(cookie); it != mvdic_.end()) {
	    auto& mv = (*it).second;
	    p_("    move_to: found: cookie:%{}, from_wd:%{}, from_name:%{}",
	       cookie, mv.from_wd, mv.from_name);
	    mv.to_wd = to_wd;
	    mv.to_name = to_name;
	    return true;
	} else {
	    return false;		// require add_watch & add_dir
	}
    }

    bool move_self(int self_wd) {
	auto& swd = wdv_[self_wd];
	p_("    move_self: mvdic_.size:%{}", mvdic_.size());
	for (auto& [ck, mv]: mvdic_) {
	    p_("    move_self: swd.parent_wd:%{}, swd.name:%{}, from_wd:%{}, from_name:%{}",
	       swd.parent_wd, swd.name, mv.from_wd, mv.from_name);
	    if (swd.parent_wd == mv.from_wd && swd.name == mv.from_name) {
		p_("    move_self: found: from_wd:%{}, from_name:%{}, to_wd:%{}, to_name:%{}",
		   mv.from_wd, mv.from_name, mv.to_wd, mv.to_name);
		if (mv.to_name.size() > 0) {
		    swd.parent_wd = mv.to_wd;
		    swd.name      = mv.to_name;
		    mvdic_.erase(ck);
		    return true;
		} else {
		    return false;	// require rm_watch & rm_dir
		}
	    }
	}
	return true;
    }

    std::string path(int trg_wd) {
	std::vector<int> dv;
	for (int wd = trg_wd; wd != -1; wd = wdv_[wd].parent_wd) {
	    dv.push_back(wd);
	}
	std::string s;
	for (auto wd: dv | c7::nseq::reverse()) {
	    s += wdv_[wd].name;
	    s += '/';
	}
	s.resize(s.size()-1);
	return s;
    }

    std::string path(int parent_wd, const std::string& name) {
	auto s = path(parent_wd);
	s += '/';
	s += name;
	return s;
    }

private:
    struct watched_dir {
	int parent_wd = -1;
	std::string name;
    };
    struct moving_dir {
	int from_wd = -1;
	int to_wd   = -1;
	std::string from_name;
	std::string to_name;
    };

    std::vector<watched_dir> wdv_;
    std::unordered_map<uint32_t, moving_dir> mvdic_;
};

static inotify_dirs dirman;


static std::string in_mask_str(uint32_t mask)
{
    static struct attr_t {
	uint32_t m;
	std::string s;
    } attrs[] = {
#define attr_(n)	{ IN_##n, ",IN_" #n }
	attr_(ACCESS),
	attr_(ATTRIB),
	attr_(CLOSE_WRITE),
	attr_(CLOSE_NOWRITE),
	attr_(CREATE),
	attr_(DELETE),
	attr_(DELETE_SELF),
	attr_(MODIFY),
	attr_(MOVE_SELF),
	attr_(MOVED_FROM),
	attr_(MOVED_TO),
	attr_(OPEN),
	attr_(IGNORED),
    };

    std::string s;
    for (auto& a: attrs) {
	if ((mask & a.m) != 0) {
	    mask &= ~a.m;
	    s += a.s;
	}
    }
    if (mask != 0) {
	s += c7::format(",0x%{x}", mask);
    }
    return s.substr(1);
}




static void handle(inotify_event& inev)
{
    p_("handle: wd:%{}, mask:%{}, cookie:%{}, len:%{}, name:<%{}>, dir:<%{}>",
       inev.wd, in_mask_str(inev.mask), inev.cookie, inev.len, inev.name,
       dirman.path(inev.wd));

    if ((inev.mask & (IN_ISDIR|IN_MOVE_SELF)) == 0) {
	return;
    }

    // directory management

    if ((inev.mask & IN_MOVED_FROM) != 0) {
	dirman.moved_from(inev.wd, inev.cookie, inev.name);
	return;
    } else if ((inev.mask & IN_MOVED_TO) != 0) {
	if (dirman.moved_to(inev.wd, inev.cookie, inev.name)) {
	    return;
	}
	inev.mask |= IN_CREATE;
    } else if ((inev.mask & IN_MOVE_SELF) != 0) {
	if (dirman.move_self(inev.wd)) {
	    return;
	}
	inev.mask |= IN_DELETE_SELF;
	in_provider->rm_watch(inev.wd);
    }

    if ((inev.mask & (IN_CREATE)) != 0) {
	auto dir = dirman.path(inev.wd, inev.name);
	if (auto res = in_provider->add_watch(dir, IN_ALL_EVENTS, handle); !res) {
	    p_("ERROR: %{}", res);
	} else {
	    dirman.add_dir(inev.wd, res.value(), inev.name);
	    p_("  add_dir: parent:%{}, self_wd:%{}, name:%{}, path:%{}",
	       inev.wd, res.value()(), inev.name, dir);
	}
    }

    if ((inev.mask & (IN_DELETE_SELF|IN_IGNORED)) != 0) {
	auto dir = dirman.path(inev.wd);
	dirman.rm_dir(inev.wd);
	p_("  rm_dir: self_wd:%{}, name:%{}, path:%{}",
	   inev.wd, inev.name, dir);
	// directory has been remove from watch list by kernel.
    }

}


int main()
{
    if (auto res = c7::event::inotify_provider::make_and_manage(); !res) {
	c7abort(res);
    } else {
	in_provider = std::move(res.value());
    }

    if (auto res = in_provider->add_watch(".", IN_ALL_EVENTS, handle); !res) {
	c7error(res);
    } else {
	dirman.add_route(res.value(), ".");
	p_("add_root: self_wd:%{}", res.value()());
    }

    c7::event::forever();
}

