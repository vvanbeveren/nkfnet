/*
 * SocketException.h
 *
 *  Created on: 8 nov. 2011
 *      Author: vincentb
 */

#ifndef SOCKETEXCEPTION_H_
#define SOCKETEXCEPTION_H_

#include <stdexcept>
#include <string>
#include "net.h"

/** \file */

START_NKF_NET

/**
 * SocketException is thrown is an socket error occurs.
 */
class NKFNET_API SocketException: public std::runtime_error {

private:
	int _code;
public:
	/**
	 * Copy constructor.
	 */
	SocketException(const SocketException& se);

	/**
	 * Creates a new SocketException.
	 * \param	message		human readable message.
	 * \param	nvErrCode	native error code.
	 */
	SocketException(std::string message, int nvErrCode);

	/**
	 * Returns the native error code.
	 *
	 * \return	Native error code.
	 */
	int code();

	/**
	 * Raises the last socket error as an exception.
	 */
	static void raiseLastError();
};


END_NKF_NET

#endif /* SOCKETEXCEPTION_H_ */
