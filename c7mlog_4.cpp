#include <algorithm>
#include <cstring>
#include <random>
#include <c7mlog.hpp>
#include <c7seq.hpp>
#include <c7threadsync.hpp>
#include <c7utils.hpp>

using c7::p_;

static void partition_test()
{
    uint32_t flags = (C7_MLOG_F_THREAD_NAME |
		      C7_MLOG_F_SOURCE_NAME);

    std::vector<size_t> sizev = {2048, 0, 0, 0, 2048, 0, 2048};
    c7::mlog_writer mlog;
    auto res = mlog.init("c7xxdev", 0, sizev, flags);
    if (!res) {
	p_("writer: %{}", res);
	return;
    }

    std::vector<int> levels = {
	0, 1, 2, 2, 3, 4, 4, 4, 4, 4, 4, 5, 5,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    };
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    size_t mdata = 0;
    
    for (int i = 0; i < 5; i++) {
	std::shuffle(levels.begin(), levels.end(), engine);
	for (auto level: levels) {
	    mdata++;
	    mlog.format(__FILE__, __LINE__, level, 0, mdata, "#%{}, level:%{}", mdata, level);
	}
    }

    p_("-- scan --");
    c7::mlog_reader reader;
    res = reader.load("c7xxdev");
    if (!res) {
	p_("reader: %{}", res);
	return;
    }

    reader.scan(0, 0, 0,
		[](auto&) { return true; },
		[](const c7::mlog_reader::info_t& info, void *data) {
		    p_("order:%{}, mdata:%{}, time_us:%{t}, level:%{}, <%{}>",
		       info.weak_order, info.minidata, info.time_us, info.level,
		       static_cast<char*>(data));
		    return true;
		});
}


int main()
{
    partition_test();
    return 0;
}
