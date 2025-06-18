#include <c7app.hpp>
#include <c7path.hpp>
#include <c7socket.hpp>
#include <cctype>
#include <cstring>
#include <unistd.h>

static void server(const std::string& addr)
{
    c7::socket sock;
    if (auto res = c7::unix_dg_binded(addr); !res) {
	c7error(res);
    } else {
	sock = std::move(res.value());
    }

    c7::sockaddr_gen peer;
    char buffer[512];
    auto iores = sock.recvfrom(buffer, sizeof(buffer), peer);
    if (!iores) {
	c7error("recvfrom failed: %{}", iores);
    }

    c7echo("peer: %{}", peer);
    c7echo("recv: %{}", buffer);

    for (size_t i = 0; i < iores.get_done(); i++) {
	buffer[i] = std::toupper(buffer[i]);
    }

    iores = sock.sendto(buffer, iores.get_done(), peer);
    if (!iores) {
	c7error("sendto failed: %{}", iores);
    }
}

static void client(const std::string&addr)
{
    c7::socket sock;
    if (auto res = c7::unix_dg_binded(); !res) {
	c7error(res);
    } else {
	sock = std::move(res.value());
    }
    if (auto res = sock.self(); res) {
	c7echo("self: %{}", res.value());
    }

    auto res = c7::sockaddr_unix(addr);
    if (!res) {
	c7error(res);
    }
    auto& peer = res.value();
    c7echo("peer: %{}, family: %{}", peer, peer.unix.sun_family);

    char buffer[512];
    std::strcpy(buffer, "hello world");

    auto iores = sock.sendto(buffer, std::strlen(buffer)+1, peer);
    if (!iores) {
	c7error("sendto failed: %{}", iores);
    }

    iores = sock.recvfrom(buffer, sizeof(buffer), peer);
    if (!iores) {
	c7error("recvfrom failed: %{}", iores);
    }
    c7echo("actual peer: %{}, family: %{}", peer, peer.unix.sun_family);

    c7echo("recv: %{}", buffer);
}

int main(int argc, char **argv)
{
    if (argc != 3) {
	c7exit("Usage: %{} addr s|c", c7::app::progname);
    }

    std::string addr{" "};
    addr += argv[1];
    addr[0] = 0;

    if (*argv[2] == 's') {
	server(addr);
    } else {
	client(addr);
    }
}

