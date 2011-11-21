/*
 * SocketException.cpp
 *
 *  Created on: 8 nov. 2011
 *      Author: vincentb
 */

#include "SocketException.h"
#include <cstring>

START_NKF_NET

// --------------------------------------------------------------------------
// SocketException
// --------------------------------------------------------------------------

SocketException::SocketException(const SocketException & se) :
	std::runtime_error(se.what()), _code(se._code) {
}
;

SocketException::SocketException(std::string message, int nvErrCode) :
	std::runtime_error(message), _code(nvErrCode) {
}
;

int SocketException::code() {
	return _code;
}
;

void SocketException::raiseLastError() {
#ifdef _WIN32
	int code = WSAGetLastError();
	if (code == 0) {
		return;
	}
	std::string msg;
	LPSTR lpMsgBuf;
	FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
					| FORMAT_MESSAGE_IGNORE_INSERTS, NULL, code,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0,
			NULL);
	msg = lpMsgBuf;

	LocalFree(lpMsgBuf);
	if (msg.length() > 2) {
		msg = msg.substr(0, msg.length() - 2); // remove line break
	}
#else
	int code = errno;
	if (code == 0) {
		return;
	}

	std::string msg = strerror(errno);
#endif
	throw SocketException(msg, code);
}



END_NKF_NET

