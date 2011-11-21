/*
 * net.cpp
 *
 *  Created on: 8 nov. 2011
 *      Author: vincentb
 */

#include "net.h"
#include "SocketException.h"


// --------------------------------------------------------------------------
// WinSock Management (duh!)
// --------------------------------------------------------------------------

#ifdef WIN32_API
START_NKF_NET


void WSockMgr::incRef() {
	if (_wsCount++ == 0) initWinsock();
}

void WSockMgr::decRef() {
	if (--_wsCount == 0) deinitWinsock();
}

void WSockMgr::initWinsock() {
	WSADATA wsadata;
	int error = WSAStartup(0x0202, &wsadata);
	if (error != 0) {
		throw SocketException("Failed to start Winsock2", error);
	}
};

void WSockMgr::deinitWinsock() {
		WSACleanup();
};

int WSockMgr::_wsCount = 0;

END_NKF_NET

#endif
