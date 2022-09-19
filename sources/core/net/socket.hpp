#ifndef STRAITX_SOCKET_HPP
#define STRAITX_SOCKET_HPP

#include "core/types.hpp"
#include "core/net/ip.hpp"

using SocketHandle = u64;

class Socket {
public:
	static constexpr u16 AnyPort = 0;
protected:
	static constexpr SocketHandle InvalidSocket = -1;
protected:
	static SocketHandle OpenImpl(bool is_udp);

	static void CloseImpl(SocketHandle socket);

	static bool BindImpl(SocketHandle socket, IpAddress address, u16 port_hbo);

	static void SetBlocking(SocketHandle socket, bool is_blocking);
};

#endif//STRAITX_SOCKET_HPP