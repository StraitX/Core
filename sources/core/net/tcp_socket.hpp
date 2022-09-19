#ifndef STRAITX_TCP_SOCKET_HPP
#define STRAITX_TCP_SOCKET_HPP

#include "core/net/socket.hpp"

class TcpSocket: public Socket{
private:
	SocketHandle m_Handle = InvalidSocket;
	IpAddress m_RemoteIpAddress = IpAddress::Any;
	u16 m_RemotePort = 0;
public:
	TcpSocket() = default;

	TcpSocket(SocketHandle handle, IpAddress remote_address, u16 remote_port);

	TcpSocket(TcpSocket &&)noexcept;

	~TcpSocket();

	TcpSocket &operator=(TcpSocket &&)noexcept;

	bool Connect(IpAddress address, u16 port_hbo);

	void Disconnect();
	
	//send and receive return less than size only if disconnected or if socket is non-blocking
	u32 Send(const void *data, u32 size);
	
	// if return < size then (!IsConnected => disconnected) ? not ready : disconnected
	u32 Receive(void *data, u32 size);

	bool IsConnected()const;

	u16 RemotePort()const;

	IpAddress RemoteIpAddress()const;

	void SetBlocking(bool is_blocking) { Socket::SetBlocking(m_Handle, is_blocking); }
private:
	void MakeValid();

	void MakeInvalid();

	bool IsValid()const;

	static bool ConnectImpl(SocketHandle socket, IpAddress address, u16 port_hbo);

	static u32 SendImpl(SocketHandle socket, const void *data, u32 size, bool &is_disconnected);

	static u32 ReceiveImpl(SocketHandle socket, void *data, u32 size, bool &is_disconected);
};

#endif//STRAITX_TCP_SOCKET_HPP