#include <c7mlog.hpp>
#include <c7seq.hpp>
#include <c7threadsync.hpp>
#include <c7utils.hpp>
#include <cstring>

using c7::p_;

static void thread_test()
{
    const int duration_ms = 500;

    p_("-- thread ----------------------------------------------");

    uint32_t flags = (C7_MLOG_F_THREAD_NAME |
		      C7_MLOG_F_SOURCE_NAME |
		      C7_MLOG_F_SPINLOCK);

    c7::mlog_writer mlog;
    auto res = mlog.init("c7xxdev", 128, 64*1024, flags);
    if (!res) {
	p_("writer: %{}", res);
	return;
    }
    (void)std::strcpy((char *)mlog.hdraddr(), "c7xxdev");

    c7::thread::group grp;
    std::vector<c7::thread::thread> thv(3);
    static std::atomic<bool> stop_thread = false;

    std::function<void(int, std::string)>
	logger([&mlog](auto i, auto name) {
		for (uint32_t n = 0;; n++) {
		    size_t mdata = (((size_t)i)<<32)|n;
		    mlog.format(__FILE__, __LINE__, 0, 0, mdata, "%{}: %{}", name, n);
		    if (stop_thread) {
			p_("%{} stopped at %{}", name, n);
			return;
		    }
		}
	    });

    for (auto [i, th]: c7::seq::enumerate(thv)) {
	grp.add(th);
	th.target(logger, i, c7::format("thread#%{}", i));
	th.start();
    }

    grp.start();
    c7::sleep_ms(duration_ms);
    stop_thread = true;
    grp.wait();

    p_("-- scan --");
    c7::mlog_reader reader;
    res = reader.load("c7xxdev");
    if (!res) {
	p_("reader: %{}", res);
	return;
    }

    for (size_t i = 0; i < thv.size(); i++) {
	uint64_t order;
	uint32_t id = 0;
	uint32_t mdata = 0;
	p_("-- scan %{} ...", i);
	reader.scan(0, 0, 0,
		    [i](const c7::mlog_reader::info_t& info) {
			uint32_t id = info.minidata >> 32;
			return (id == (uint32_t)i);
		    },
		    [&mdata, &id, &order](const c7::mlog_reader::info_t& info, void *) {
			if (mdata == 0) {
			    order = info.weak_order;
			    id    = info.minidata >> 32;
			    mdata = (info.minidata << 32) >> 32;
			    return true;
			} else {
			    uint64_t o_order = order;
			    uint32_t o_id = id;
			    uint32_t o_mdata = mdata;
			    order = info.weak_order;
			    id    = info.minidata >> 32;
			    mdata = (info.minidata << 32) >> 32;
			    if (o_mdata + 1 == mdata) {
				return true;
			    }
			    p_("[%{}] #%{} %{}:%{} -> #%{} %{}:%{}",
			       info.thread_id,
			       o_order,
			       o_id,
			       o_mdata,
			       order,
			       id,
			       mdata);
			    return true;
			}
		    });
	p_("-- scan %{} ... mdata:%{}, order:%{}", i, mdata, order);
    }
}


int main()
{
    thread_test();
    return 0;
}
