/* Part of nkflib
 *
 * Copyright 2011 NIKHEF Amsterdam
 *
 * Socket.h
 *
 *  Created on: 2 nov. 2011
 *      Author: V. van Beveren
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include "net.h"
#include "Address.h"

/** \file */

START_NKF_NET

/**
 * The socket type to create.
 */
enum SocketType {
	TCP /* < A TCP IP socket */,
	UDP /* < A UDP socket */
};

/**
 * Convenience function to make a timeval structure.
 *
 * \param	secs	The seconnds of the time value.
 * \param	usecs	The microseconds of the time value.
 * \return			A timeval structure.
 */
inline timeval mktv(unsigned long secs, unsigned long usecs)
{
	timeval tv;
	tv.tv_sec = secs;
	tv.tv_usec = usecs;
	return tv;
}

/**
 * Indicates a waiting time of 'forever'.
 * (which is about 136 years, in this case).
 */
static const timeval FOREVER = { 0xffffffff, 0xffffffff };


/**
 * Socket is a small cross-platform socket abstraction, which aims to
 * unify sockets across several platforms. Currently only TCP and UDP
 * is supported.
 */
class NKFNET_API Socket {
public:
	/**
	 * Create a new socket of the specific type.
	 * \param   type  The socket type, either UDP and TCP.
	 */
	Socket(SocketType type);

	/**
	 * Creates a socket with the specified handle / file descriptor.
	 *
	 * *Use this only when you really really need it*.
	 *
	 * \param	handle	The handle / file descriptor of this socket.
	 */
	Socket(SOCKET handle);

	/**
	 * Closes the socket explicitly. Note that after this the socket
	 * is no longer valid and can not be used.
	 *
	 * Calling this function multiple times has no effect.
	 */
	void	close();

	/**
	 * Destroys the socket, freeing any resources it may still hold.
	 */
	virtual	~Socket();

	/**
	 * Sets a socket option.
	 *
	 * Uses constants from Winsock or Posix sockets.
	 * See the WinSock reference, of the MAN-pages.
	 *
	 * Special note for SO_RCVTIMEO and SO_SNDTIMEO on windows, the option
	 * accepted is a timeval structure, not a DWORD (unsigned long), as
	 * in the WinSock documentation. Passing anything else may yield to
	 * undesirable results. On Windows this option has at most 1ms resolution.
	 *
	 * \param	level	The level, e.g. SOL_SOCKET.
	 * \param	option	The option to set, see SO_* constants
	 * \param	value	The value to set
	 * \param	size	The size of the value in bytes.
	 */
	void	setOption(int level, int option, const void * value, size_t size);

	/**
	 * Sets a boolean socket option.
	 *
	 * Uses constants from Winsock or Posix sockets.
	 * See the WinSock reference, of the MAN-pages.
	 *
	 * This a convenience function and uses SOL_SOCKET as the level.
	 *
	 * \param	option	The option to set, see SO_* constants
	 * \param	value	The value to set, in this case a boolean
	 */
	void	setOption(int option, bool value) {
		setOption(SOL_SOCKET, option, &value, sizeof(value));
	}

	/**
	 * Sets an int socket option.
	 *
	 * Uses constants from Winsock or Posix sockets.
	 * See the WinSock reference, of the MAN-pages.
	 *
	 * This a convenience function and uses SOL_SOCKET as the level.
	 *
	 * \param	option	The option to set, see SO_* constants
	 * \param	value	The value to set, in this case a integer
	 */
	void	setOption(int option, int value) {
		setOption(SOL_SOCKET, option, &value, sizeof(value));
 	}

	/**
	 * Sets a timeval socket option.
	 *
	 * Uses constants from Winsock or Posix sockets.
	 * See the WinSock reference, of the MAN-pages.
	 *
	 * Special note for SO_RCVTIMEO and SO_SNDTIMEO on windows, the option
	 * accepted is a timeval structure, not a DWORD (unsigned long), as
	 * in the WinSock documentation.
	 * On Windows this option has at most 1ms resolution.
	 *
	 * This a convenience function and uses SOL_SOCKET as the level.
	 *
	 * \param	option	The option to set, see SO_* constants
	 * \param	value	The value to set, in this case timeval
	 */
	void	setOption(int option, timeval value) {
		setOption(SOL_SOCKET, option, &value, sizeof(value));
 	}

	/**
	 * Gets a socket option, constants used are from Winsock or Posix sockets.
	 *
	 * Uses constants from Winsock or Posix sockets.
	 * See the WinSock reference, of the MAN-pages.
	 *
	 * Special note for SO_RCVTIMEO and SO_SNDTIMEO on windows, the option
	 * accepted is a timeval structure, not a DWORD (unsigned long), as
	 * in the WinSock documentation. Expect a timeval to be returned.
	 *
	 * \param	level	The level, e.g. SOL_SOCKET.
	 * \param	option	The option to set, see SO_* constants
	 * \param	value	The value to set
	 * \param	size	The size of the value in bytes.
	 */
	void	getOption(int level, int option, void * value, size_t * size);


	/**
	 * Gets a socket option as a bool.
	 *
	 * Uses constants from Winsock or Posix sockets.
	 * See the WinSock reference, of the MAN-pages.
	 *
	 * This a convenience function and uses SOL_SOCKET as the level and returns
	 * a boolean. It can only be used with options that return such a type.
	 *
	 * \param	option	The option to get, see SO_* constants
	 */
	bool	getBoolOption(int option) {
		bool val; size_t size = sizeof(val);
		getOption(SOL_SOCKET, option, &val, &size);
		return val;
	}


	/**
	 * Gets a socket option as int.
	 *
	 * Uses constants from Winsock or Posix sockets.
	 * See the WinSock reference, of the MAN-pages.
	 *
	 * This a convenience function and uses SOL_SOCKET as the level and returns
	 * a boolean. It can only be used with options that return such a type.
	 *
	 * \param	option	The option to get, see SO_* constants
	 */
	int		getIntOption(int option) {
		int val;
		size_t size = sizeof(val);
		getOption(SOL_SOCKET, option, &val, &size);
		return val;
	}

	/**
	 * Gets a socket option as timeval.
	 *
	 * Uses constants from Winsock or Posix sockets.
	 * See the WinSock reference, of the MAN-pages.
	 *
	 * This a convenience function and uses SOL_SOCKET as the level and returns
	 * a timeval. It can only be used with options that return such a type.
	 *
	 * Special note for SO_RCVTIMEO and SO_SNDTIMEO on windows, the option
	 * accepted is a timeval structure, not a DWORD (unsigned long), as
	 * in the WinSock documentation. Expect a timeval to be returned.
	 *
	 * \param	option	The option to get, see SO_* constants
	 */
	timeval	getTimevalOption(int option) {
		timeval val; size_t size = sizeof(val);
		getOption(SOL_SOCKET, option, &val, &size);
		return val;
	}


	/**
	 * Connects to a specific hostname and port.
	 *
	 * \param	addr		The remote address to connect to.
	 */
	void	connect(const Address & addr);

	/**
	 * Bind to a specific adapter, by address. Address must be a
	 * local host address. Pass Socket::ANY if you want to bind
	 * to any adapter.
	 *
	 * \param	addr		The local address of the adapter to bind to.
	 */
	void	bind(const Address & addr);

	/**
	 * Sends to the connected host, fails if no connection has been made.
	 *
	 * \param	buf		The buffer to send.
	 * \param	len		The length of the buffer in bytes.
	 *
	 * \return			The number of bytes send, or 0 if none.
	 */
	size_t	send(const void * buf, size_t len);

	/**
	 * Sends to the host specified at address.
	 *
	 * \param	buf		The buffer to send.
	 * \param	len		The length of the buffer in bytes.
	 * \param	addr	The address to send the data to.
	 *
	 * \return			The number of bytes send, or 0 if none
	 *
	 */
	size_t	send(const void * buf, size_t len, const Address & addr);


	/**
	 * Receive data into the given buffer.
	 *
	 * \param	buf		The buffer to receive in.
	 * \param	len		The length of the buffer in bytes.
	 *
	 * \return			The number of bytes received, never exceeds len.
	 */
	size_t	receive(void * buf, size_t len);


	/**
	 * Receive data into the given buffer, and supply specific address.
	 *
	 * \param	buf		The buffer to receive in.
	 * \param	len		The length of the buffer in bytes.
	 * \param	addr	The address from which the data was received.
	 *
	 * Passing the address is not really nice, cause Address should
	 * really be a value object. You could really mess up things by
	 * doing stuff like:
	 * \code s.receive(&myBuf, sizeof(myBuf), &Address::ANY) \endcode
	 *
	 * However, good usage would be for receiving UDP packets:
	 *
	 * \code
	 * Socket s(UDP);
	 * s.bind(IP4(127,0,0,1), 12345);
	 * Address from;
	 * char buf[1024];
	 * size_t received;
	 * while (1) {
	 *   received = s.receive(&buf, sizeof(buf), &from);
	 *   cout << "Received " << received << " bytes from " << from.toString() << endl;
	 * }
	 * \endcode
	 *
	 * \return			The number of bytes received, never exceeds len.
	 */
	size_t	receive(void * buf, size_t len, Address * addr);

	/**
	 * Puts the socket into a listening state.
	 *
	 * \param	backlog		Max of pending connections, by default the
	 * 						backlog is chosen by the driver.
	 */
	void	listen(int backlog = SOMAXCONN);

	/**
	 * Accepts incoming new connections. Blocks indefinitely if no
	 * timeout was set.
	 *
	 * \return	A new Socket.
	 */
	Socket*	accept();

	/**
	 * Returns the local address this socket is connected to.
	 *
	 * \return	The local address.
	 */
	Address localAddress();

	/**
	 * Returns the remote (peer) address this socket is connected to,
	 * if any.
	 *
	 * \return	The remote address.
	 */
	Address remoteAddress();


	/**
	 * Sets blocking IO on or off.
	 *
	 * \param	blocking	If true, the socket will be blocking.
	 * 						If false, the socket will be non-blocking.
	 */
	void	setBlocking(bool blocking);


	/**
	 * Returns the OS-specific socket handle.
	 *
	 * \return	The socket handle i.e. file descriptor.
	 */
	SOCKET	handle();

private:


	SOCKET _handle;

	Address _local;

	Address _remote;

};

END_NKF_NET
#endif /* SOCKET_H_ */
