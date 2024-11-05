#include <c7app.hpp>
#include <c7mlog.hpp>
#include <c7utils.hpp>
#include <cstring>


static c7::mlog_writer mlog_writer;
#define mlog(g, ...)	mlog_writer.format(__FILE__, __LINE__, C7_LOG_##g, 0, 0, __VA_ARGS__)

static void callback(c7::usec_t time_us, const char *src_name, int src_line,
		     uint32_t level, uint32_t category, uint64_t minidata,
		     const void *logaddr, size_t logsize_b)
{
    c7echo("%{t} callback ... !!!", time_us);
}

int main(int argc, char **argv)
{
    mlog(ALW, "hello: %{}", *argv);
    mlog_writer.set_callback(nullptr);
    mlog(ALW, "hello: %{}", *argv);
    mlog_writer.set_callback(callback);
    mlog(ALW, "hello: %{}", *argv);
    return 0;
}
