#include "core/net/udp_socket.hpp"
#include "core/move.hpp"
#include "core/algorithm.hpp"
#include "core/assert.hpp"

UdpSocket::UdpSocket() {
	Open();
}

UdpSocket::UdpSocket(UdpSocket&& other)noexcept{
	*this = Move(other);
}

UdpSocket::~UdpSocket(){
	Close();
}

UdpSocket& UdpSocket::operator=(UdpSocket&& other)noexcept{
	Close();
	Swap(m_Handle, other.m_Handle);
	Swap(m_IsBound, other.m_IsBound);
	return *this;
}

void UdpSocket::Open() {
	if(IsOpen())
		return;

	m_Handle = OpenImpl(true);
}

void UdpSocket::Close(){
	if(!IsOpen())
		return;

	CloseImpl(m_Handle);
	m_Handle = InvalidSocket;
}

bool UdpSocket::Bind(IpAddress address, u16 port_hbo) {
	SX_CORE_ASSERT(IsOpen(), "Can't bind closed UdpSocket");
	SX_CORE_ASSERT(!IsBound(), "UdpSocket can be bound only once");

	return (m_IsBound = BindImpl(m_Handle, address, port_hbo));
}

bool UdpSocket::IsOpen()const{
	return m_Handle != InvalidSocket;
}

bool UdpSocket::IsBound()const {
	return m_IsBound;
}

u32 UdpSocket::Send(const void* data, u32 size, IpAddress dst_ip, u16 dst_port_hbo) {
	SX_CORE_ASSERT(IsOpen(), "Can't send on a closed UdpSocket");
	m_IsBound = true;

	return SendImpl(m_Handle, data, size, dst_ip, dst_port_hbo);
}

u32 UdpSocket::Receive(void* data, u32 size, IpAddress& src_ip, u16& src_port_hbo) {
	SX_CORE_ASSERT(IsOpen(), "Can't receive on closed UdpSocket");
	SX_CORE_ASSERT(IsBound(), "Can't receive on unbound UdpSocket");

	return ReceiveImpl(m_Handle, data, size, src_ip, src_port_hbo);
}
