#include "core/net/tcp_listener.hpp"
#include "core/algorithm.hpp"

TcpListener::TcpListener(){
	m_Handle = Socket::OpenImpl(false);
}

TcpListener::TcpListener(TcpListener &&other)noexcept{
	*this = Move(other);
}

TcpListener::~TcpListener(){
	MakeInvalid();
}

bool TcpListener::Bind(IpAddress address, u16 port){
	return Socket::BindImpl(m_Handle, address, port) && ListenImpl(m_Handle);
}

TcpSocket TcpListener::Accept(){
	IpAddress addr = IpAddress::Any;
	u16 port = 0;
	SocketHandle connection = AcceptImpl(m_Handle, addr, port);
	return {connection, addr, port};
}

void TcpListener::MakeInvalid(){
	if(IsValid()){
		Socket::CloseImpl(m_Handle);
		m_Handle = InvalidSocket;
	}
}

TcpListener& TcpListener::operator=(TcpListener &&other)noexcept{
	MakeInvalid();
	Swap(other.m_Handle, m_Handle);
	return *this;
}

