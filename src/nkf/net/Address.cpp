/*
 * Address.cpp
 *
 *  Created on: 8 nov. 2011
 *      Author: vincentb
 */

#include "Address.h"
#include "SocketException.h"
#include <iostream>
#include <sstream>
#include <cstring>

START_NKF_NET

// --------------------------------------------------------------------------
// Address
// --------------------------------------------------------------------------
Address::Address(unsigned long addr, unsigned short port) :
	_addr(reinterpret_cast<sockaddr*> (&_addrIn)) {
	init(addr, port);
}

// --------------------------------------------------------------------------

Address::Address(unsigned short port) :
	_addr(reinterpret_cast<sockaddr*> (&_addrIn)) {
	init(INADDR_ANY, port);
}

// --------------------------------------------------------------------------

Address::Address(std::string addr, unsigned short port) :
	_addr(reinterpret_cast<sockaddr*> (&_addrIn)) {

	INC_WS_REF	// winsock needs to be initialized to use this

	struct addrinfo *result;

	RoR(::getaddrinfo(addr.c_str(), NULL, NULL, &result));

	memcpy(&_addrIn, result->ai_addr, sizeof(_addr));

	::freeaddrinfo(result);

	DEC_WS_REF
}

// --------------------------------------------------------------------------

Address::Address(const Address & addr) :
	_addr(reinterpret_cast<sockaddr*> (&_addrIn)) {
	init(ntohl(addr._addrIn.sin_addr.s_addr), ntohs(addr._addrIn.sin_port));
}

// --------------------------------------------------------------------------

void Address::init(unsigned long ip4addr, unsigned short port) {
	memset(&_addrIn, 0, sizeof(_addrIn));
	_addrIn.sin_family		= AF_INET;
	_addrIn.sin_addr.s_addr	= htonl(ip4addr);
	_addrIn.sin_port		= htons(port);
}

// --------------------------------------------------------------------------

bool  Address::isNull()
{
	return (_addrIn.sin_addr.s_addr == 0 && _addrIn.sin_port == 0);
}

// --------------------------------------------------------------------------

unsigned short Address::port()
{
	return ntohs(_addrIn.sin_port);
}

// --------------------------------------------------------------------------

unsigned long Address::ip4addr()
{
	return ntohl(_addrIn.sin_addr.s_addr);
}

// --------------------------------------------------------------------------

std::string Address::toString() {
	unsigned long ip4 = ip4addr();
	std::ostringstream ss;
	ss << ( (ip4 >> 24) & 0xff) << ".";
	ss << ( (ip4 >> 16) & 0xff) << ".";
	ss << ( (ip4 >> 8) & 0xff) << ".";
	ss << ( (ip4 >> 0) & 0xff) << ":";
	ss << ntohs(_addrIn.sin_port);
	return ss.str();
}

// --------------------------------------------------------------------------

const Address Address::ANY = Address(0);

END_NKF_NET
