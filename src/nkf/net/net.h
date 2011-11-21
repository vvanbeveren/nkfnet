/*
 * net.h
 *
 *  Created on: 8 nov. 2011
 *      Author: vincentb
 */

/*!
 * \mainpage The NIKHEF standard C++ net library.
 *
 * This package contains the NIKHEF standard met library, a generic library used
 * to build the NIKHEF software.
 *
 * This library contains a cross-platform Socket abstraction, which is a thin OO
 * layer around for Berkeley(-like) sockets libraries.
 *
 * The following code makes a request to Google for the start page, and receives
 * the reply.
 *
 * \code
 * Socket s(TCP);
 * s.connect(Address("www.google.com", 80));
 * std::string req = "GET / HTTP/1.0\n";
 * char buf[1024];
 * s.send(req.c_str(), req.length());
 * s.receive(&buf, len(buf));
 * s.close();
 * \endcode
 *
 * Note that some terminology is different:
 * - file descriptor (fd) has been renamed to handle (though you don't have much)
 *   to do with it.
 * - fd_set is now a SocketSet.
 * - 'peer' address has become 'remote' address
 * - 'socket name' has become 'local' address.
 * - Some abbreviated function names have been expanded, e.g. 'recv' is 'receive'.
 */

#ifndef NET_H_
#define NET_H_

#include "../defines.h"

#ifdef WIN32_API
#include <winsock2.h>
#include <Ws2tcpip.h>
// For compatibility with Linux
typedef int socklent_t;
#else
// Linux
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
// For compatibility with Windows
typedef int SOCKET;
#define INVALID_SOCKET -1
#endif

#define START_NKF_NET	\
namespace nkf {			\
namespace net {

#define END_NKF_NET		\
}						\
}

#ifdef WIN32_API

START_NKF_NET

/* 
 WinSockMgr keeps Winsock happy. It initializes
   and deinitializes WinSock. With special thanks
   to Microsoft.
 */

class WSockMgr {

public:
	static void incRef();
	static void decRef();

private:
	static int _wsCount;

	static void initWinsock();
	static void deinitWinsock();
};

#define	INC_WS_REF	WSockMgr::incRef();
#define	DEC_WS_REF	WSockMgr::decRef();

END_NKF_NET

#else

#define	INC_WS_REF
#define	DEC_WS_REF
#endif

/* 
  Simple macro to throw an error when there is one 
  RoR means Raise on eRror
 */
#define RoR(func) if ( ( func ) != 0 ) SocketException::raiseLastError()

#endif /* NET_H_ */
