#ifndef STRAITX_TCP_LISTENER_HPP
#define STRAITX_TCP_LISTENER_HPP

#include "core/net/socket.hpp"
#include "core/net/tcp_socket.hpp"

class TcpListener: public Socket{
private:
	SocketHandle m_Handle = InvalidSocket;
public:
	TcpListener();

	TcpListener(TcpListener &&)noexcept;

	~TcpListener();

	TcpListener &operator=(TcpListener &&)noexcept;

	bool Bind(IpAddress address, u16 port);

	TcpSocket Accept();

	void SetBlocking(bool is_blocking) { Socket::SetBlocking(m_Handle, is_blocking); }

private:
	bool IsValid()const {
		return m_Handle != InvalidSocket;
	}

	void MakeInvalid();

	static bool ListenImpl(SocketHandle socket);

	static SocketHandle AcceptImpl(SocketHandle socket, IpAddress &src_ip, u16 &src_port_hbo);
};

#endif//STRAITX_TCP_LISTENER_HPP