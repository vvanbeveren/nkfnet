/* Part of nkflib
 *
 * Copyright 2011 NIKHEF Amsterdam
 *
 * Socket.cpp
 *
 *  Created on: 2 nov. 2011
 *      Author: V. van Beveren
 */

#include "Socket.h"
#include "SocketException.h"

START_NKF_NET

// --------------------------------------------------------------------------
// Socket
// --------------------------------------------------------------------------

Socket::Socket(SocketType st) :
	_handle(INVALID_SOCKET),
	_local(Address::ANY),
	_remote(Address::ANY) {
	INC_WS_REF

	int af = AF_INET; // address family, currently only IP v4 is supported
	int type;
	int proto = 0;

	switch (st) {
	case TCP:
		type = SOCK_STREAM;
		proto = IPPROTO_TCP;
		break;
	case UDP:
		type = SOCK_DGRAM;
		proto = IPPROTO_UDP;
		break;
	default:
		throw SocketException("Unknown socket type", 0);
	}

	_handle = socket(af, type, proto);

	if (_handle == INVALID_SOCKET) {
		SocketException::raiseLastError();
	}
}

Socket::Socket(SOCKET handle) :
		_handle(handle),
		_local(Address::ANY),
		_remote(Address::ANY) {

	if (_handle == INVALID_SOCKET) {
		throw SocketException("Provided socket has invalid handle!", 0);
	}
	INC_WS_REF
}

Socket::~Socket() {
	try {
		Socket::close();
		DEC_WS_REF
	} catch (const SocketException & se) {
		DEC_WS_REF
		throw se;
	}
}

// --------------------------------------------------------------------------

void Socket::close() {
	if (_handle == INVALID_SOCKET)
		return;

#ifdef WIN32_API
	closesocket(_handle);
#else
	::close(_handle);
#endif
	_handle = INVALID_SOCKET;

}

// -----------------------------------------------------------------------------

void Socket::setOption(int level, int option, const void * value, size_t size) {

#ifdef WIN32_API		// patch up API inconsistencies
	unsigned long llw;

	switch (option) {
	case SO_RCVTIMEO:
	case SO_SNDTIMEO:
		timeval tv = (*static_cast<const timeval*>(value));
		llw = tv.tv_usec / 1000 + tv.tv_sec * 1000;
		value = &llw;
	}
#endif
	RoR(::setsockopt(_handle, level, option, static_cast<const char*>(value), size));
}

// -----------------------------------------------------------------------------

void Socket::getOption(int level, int option, void * value, size_t * size) {
	socklen_t isize = (*size);	// patch it up safely, will else fail on 64 bit systems
	RoR(::getsockopt(_handle, level, option, static_cast<char*>(value), &isize));
	(*size) = isize;
#ifdef WIN32_API		// patch up API inconsistencies
	unsigned long llw;

	switch (option) {
	case SO_RCVTIMEO:
	case SO_SNDTIMEO:
		llw = (*static_cast<long*>(value));
		timeval tv;
		tv.tv_sec = llw / 1000;
		tv.tv_usec = ( llw % 1000 ) * 1000;
		memcpy(value, &tv, sizeof(timeval));
		(*size) = sizeof(timeval);
	}
#endif
}


// --------------------------------------------------------------------------

void Socket::connect(const Address & addr) {
	RoR(::connect(_handle, addr._addr, addr._addrSize));
	_remote = addr;
}

// --------------------------------------------------------------------------

void Socket::bind(const Address & addr) {
	RoR(::bind(_handle, addr._addr, addr._addrSize));
	_local = addr;
}

// --------------------------------------------------------------------------

size_t Socket::send(const void * buf, size_t len) {
	size_t bytes = ::send(_handle, static_cast<const char*> (buf), len, 0);
	if (bytes == INVALID_SOCKET)
		SocketException::raiseLastError();
	return bytes;
}

// --------------------------------------------------------------------------

size_t Socket::send(const void * buf, size_t len, const Address & addr) {
	size_t bytes = ::sendto(_handle, static_cast<const char*> (buf), len, 0,
			addr._addr, addr._addrSize);
	if (bytes == INVALID_SOCKET)
		SocketException::raiseLastError();
	return bytes;
}

// --------------------------------------------------------------------------

size_t Socket::receive(void * buf, size_t len) {
	size_t bytes = ::recv(_handle, static_cast<char*> (buf), len, 0);
	if (bytes == INVALID_SOCKET)
		SocketException::raiseLastError();
	return bytes;
}

// --------------------------------------------------------------------------

size_t Socket::receive(void * buf, size_t len, Address * addr) {
	size_t bytes = ::recvfrom(_handle, static_cast<char*> (buf), len, 0, addr->_addr, NULL);
	if (bytes == INVALID_SOCKET)
		SocketException::raiseLastError();
	return bytes;
}

// --------------------------------------------------------------------------

void Socket::listen(int backlog)
{
	RoR(::listen(_handle, backlog));
}

// --------------------------------------------------------------------------

Socket*	Socket::accept()
{
	SOCKET newHandle = ::accept(_handle, NULL, NULL);
	if (newHandle ==  INVALID_SOCKET)
		SocketException::raiseLastError();

	return new Socket(newHandle);
}

// --------------------------------------------------------------------------

Address Socket::localAddress()
{
	if (_local.isNull()) {
		sockaddr_in addr;
		socklen_t size = sizeof(addr);
		RoR(::getsockname(_handle, reinterpret_cast<sockaddr*>(&addr), &size));
		_local = Address(ntohl(addr.sin_addr.s_addr), ntohs(addr.sin_port));
	}
	return _local;
}

// --------------------------------------------------------------------------

Address Socket::remoteAddress()
{
	if (_remote.isNull()) {
		sockaddr_in addr;
		socklen_t size = sizeof(addr);
		RoR(::getpeername(_handle, reinterpret_cast<sockaddr*>(&addr), &size));
		_remote = Address(ntohl(addr.sin_addr.s_addr), ntohs(addr.sin_port));
	}

	return _remote;
}

// --------------------------------------------------------------------------

void Socket::setBlocking(bool blocking)
{
#ifdef WIN32_API
	unsigned long mode = blocking ? 1 : 0;
	RoR(::ioctlsocket(_handle, FIONBIO, &mode));
#else
	// set or clear the blocking flag
	int flags = fcntl(_handle,F_GETFL,0);

	flags |= O_NONBLOCK;
	if ( !blocking ) flags ^= O_NONBLOCK;

	RoR(::fcntl(_handle, F_SETFL, flags));
#endif
}

// --------------------------------------------------------------------------

SOCKET Socket::handle()
{
	return _handle;
}

END_NKF_NET
