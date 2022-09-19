#ifndef STRAITX_UDP_SOCKET_HPP
#define STRAITX_UDP_SOCKET_HPP

#include "core/net/socket.hpp"

namespace Udp {
	constexpr u32 HeaderSize = 8;
	constexpr u32 MaxDatagramSize = Ip::MaxPacketSize - Ip::HeaderSize - Udp::HeaderSize;
}//namespace Udp::

class UdpSocket: public Socket{
private:
	SocketHandle m_Handle = InvalidSocket;
	bool m_IsBound = false;
public:
	UdpSocket();

	UdpSocket(UdpSocket &&other)noexcept;

	~UdpSocket();

	UdpSocket &operator=(UdpSocket &&other)noexcept;

	void Open();

	void Close();

	bool Bind(IpAddress address, u16 port_hbo);

	bool IsOpen()const;

	bool IsBound()const;

	//we don't really need a u64 size support because datagram is 0xFFFF - 20 - 8 bytes at maximum
	u32 Send(const void *data, u32 size, IpAddress dst_ip, u16 dst_port_hbo);

	u32 Receive(void *data, u32 size, IpAddress &src_ip, u16 &src_port_hbo);

	u32 Receive(void* data, u32 size) {
		IpAddress dummy_ip = IpAddress::Any;
		u16 dummy_port;
		return Receive(data, size, dummy_ip, dummy_port);
	}

	void SetBlocking(bool is_blocking) { Socket::SetBlocking(m_Handle, is_blocking); }
private:
	static u32 SendImpl(SocketHandle socket, const void *data, u32 size, IpAddress dst_ip, u16 dst_port_hbo);

	static u32 ReceiveImpl(SocketHandle socket, void *data, u32 size, IpAddress &src_ip, u16 &src_port_hbo);
};

#endif//STRAITX_UDP_SOCKET_HPP