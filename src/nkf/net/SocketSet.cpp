/*
 * SocketSet.cpp
 *
 *  Created on: 11 nov. 2011
 *      Author: vincentb
 */

#include "SocketSet.h"
#include "SocketException.h"

START_NKF_NET

// --------------------------------------------------------------------------
// SocketSet
// --------------------------------------------------------------------------

SocketSet::SocketSet() : _fd_max(0) {
	FD_ZERO(&_fd_set);
}

// --------------------------------------------------------------------------

SocketSet::SocketSet(Socket & sock) : _fd_max(0) {
	FD_ZERO(&_fd_set);
	set(sock);
}

// --------------------------------------------------------------------------

SocketSet::SocketSet(Socket & sock1, Socket & sock2) : _fd_max(0) {
	FD_ZERO(&_fd_set);
	set(sock1);
	set(sock2);
}

// --------------------------------------------------------------------------

SocketSet::SocketSet(const SocketSet & other)
{
	_fd_set = other._fd_set;
	_fd_max = other._fd_max;
}

// --------------------------------------------------------------------------

SocketSet & SocketSet::operator=(const SocketSet & other)
{
	if (this == &other) return (*this);
	_fd_set = other._fd_set;
	_fd_max = other._fd_max;
	return (*this);
}

// --------------------------------------------------------------------------

void SocketSet::zero() {
	FD_ZERO(&_fd_set);
}

// --------------------------------------------------------------------------

void SocketSet::set(Socket & sock) {

	SOCKET handle = sock.handle();
	FD_SET(handle, &_fd_set);
	if (handle > _fd_max) {
		_fd_max = handle;
	}
}

// --------------------------------------------------------------------------

bool SocketSet::isSet(Socket & sock) {
	return FD_ISSET(sock.handle(), &_fd_set);
}

// --------------------------------------------------------------------------

int SocketSet::select(SocketSet * read, SocketSet * write, SocketSet * error, timeval & timeout)
{
	SOCKET max = 0;
	if (read != NULL && read->_fd_max > max) max = read->_fd_max;
	if (write != NULL && write->_fd_max > max) max = write->_fd_max;
	if (error != NULL && error->_fd_max > max) max = error->_fd_max;
	int r = ::select(max + 1,
			read != NULL ? &read->_fd_set : NULL,
			write != NULL ? &write->_fd_set : NULL,
			error != NULL ? &error->_fd_set : NULL,
			&timeout);
	if (r < 0) SocketException::raiseLastError();
	return r;
}

// --------------------------------------------------------------------------

SocketSet::~SocketSet() {
}

END_NKF_NET
