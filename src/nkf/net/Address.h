/*
 * Address.h
 *
 *  Created on: 8 nov. 2011
 *      Author: vincentb
 */

#ifndef ADDRESS_H_
#define ADDRESS_H_

#include <string>
#include "net.h"

/** \file */

START_NKF_NET

class Socket;

/**
 * Simple macro to generate an IP4 address, e.g.
 *
 * IP4(192, 168, 1, 1) will create an value which can
 * be passed to the Address::Address constructor.
 */
#define IP4(a1, a2, a3, a4) \
    ( ( a1 ) << 24 | ( a2 ) << 16 | ( a3 ) << 8 | ( a4 ) )


/**
 * Encapsulates an IP4 address and port.
 */
class NKFNET_API Address {

	friend class Socket;

public:
	/**
	 * Creates an address from the given unsigned integer and port number.
	 *
	 * Use the IP4 macro to make more human readable code, e.g.:
	 *
	 * \code Address addr(IP4(192,168,1,1), 1000); \endcode
	 *
	 * \param	address		The IP4 address as a long, e.g. 0x7f000001 = 127.0.0.1
	 * \param	port		The port number
	 */
	Address(unsigned long address, unsigned short port);

	/**
	 * Creates an address by its host name. Does a DNS lookup to resolve the
	 * hostname.
	 *
	 * \param	hostname	The hostname as a string, e.g. "www.google.com"
	 * \param	port		The port number
	 */
	Address(std::string hostname, unsigned short port);

	/**
	 * Creates an address with only a specific port. Useful if you wish to bind to
	 * any adapter, but on a specific port.
	 */
	Address(unsigned short port);

	/**
	 * Returns whether or not this is a null address (= unspecified / any)
	 *
	 * \return	true if both address and port are 0.
	 */
	bool	isNull();

	/**
	 * Returns the port of this address.
	 *
	 * \return	The port number.
	 */
	unsigned short port();

	/**
	 * Returns the IP address, where the high byte is the
	 * first number of the IP, i.e. to get the 168 from
	 * 192.168.0.1, you should do this:
	 *
	 * \code cout << (addr.ip4addr() >> 16) & 0xff << endl; \endcode
	 *
	 * \return	The IP address.
	 */
	unsigned long ip4addr();

	/**
	 * Returns the string representation of this address, as
	 * <a1>.<a2>.<a3>.<a4>:<port>, eg. "127.0.0.1:1234"
	 */
	std::string toString();

	/**
	 * Address constant which binds to any adapter or any port.
	 */
	static const Address ANY;


private:
	Address(const Address & addr);

	void init(unsigned long ip4addr, unsigned short port);

	unsigned long	_ip4addr;

	unsigned		_port;

	sockaddr_in		_addrIn;

	sockaddr*		_addr;

	static const int _addrSize = sizeof(sockaddr);	// convenience.
};

END_NKF_NET
#endif /* ADDRESS_H_ */
