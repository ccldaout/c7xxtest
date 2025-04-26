#include <c7app.hpp>
#include <c7socket.hpp>

#include <unistd.h>
#include <fcntl.h>
#include <c7path.hpp>

int main(int argc, char **argv)
{
    if (argc == 1 || std::strchr("sc", *argv[1]) == nullptr) {
	c7error("Usage: %{} {s | c}", c7::app::progname);
    }

    auto addr = std::string{"_descpass"};
    addr[0] = 0;

    if (*argv[1] == 's') {
	auto file = c7::path::untildize("~/tmp/descpass.data");
	c7echo("file: %{}", file);
	auto fd = std::move(c7::open(file, O_RDWR|O_CREAT|O_TRUNC).value());
	auto sock = std::move(c7::unix_server(addr).value());
	for (;;) {
	    auto ios = std::move(sock.accept().value());
	    ios.send_filedesc(fd).check();
	}
    } else {
	auto sock = std::move(c7::unix_client(addr).value());
	auto fd = c7::fd{sock.recv_filedesc().value()};
	auto s = c7::format("hello:%{}\n", ::getpid());
	fd.write(s.c_str(), s.size()).value();
    }
}
