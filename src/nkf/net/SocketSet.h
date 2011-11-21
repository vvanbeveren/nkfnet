/*
 * SocketSet.h
 *
 *  Created on: 11 nov. 2011
 *      Author: vincentb
 */

#ifndef SOCKETSET_H_
#define SOCKETSET_H_

#include "net.h"
#include "Socket.h"

START_NKF_NET

/**
 * SocketSet is a thin wrapper around the fd_set structure. It uses an OO approach, but other
 * than that, its mostly the same.
 *
 * \code
 * Socket mySock1(UDP); //...
 * Socket mySock2(UDP);
 *
 * SocketSet mySet(mySock1, mySock2);
 *
 * if (SocketSet::select(mySet, NULL, NULL, 0) > 0) {
 *   if (mySet.isSet(mySock1)) {
 *      mySock1.read(&buf, &len);	// must be mySock
 *   } else if (mySet.isSet(mySock2)) {
 *      mySock2.read(&buf, &len)
 *   } else {
 *   	// error!
 *   }
 * }
 * \endcode
 */
class NKFNET_API SocketSet {
public:
	/**
	 * Create a new empty SocketSet.
	 */
	SocketSet();

	/**
	 * Create a SocketSet with one Socket.
	 *
	 * \param	sock	The socket.
	 */
	SocketSet(Socket & sock);

	/**
	 * Create a SocketSet with two Sockets.
	 *
	 * \param	sock1	The first socket.
	 * \param	sock2	The second socket.
	 */
	SocketSet(Socket & sock1, Socket & sock2);

	/**
	 * Creates a copy of this SocketSet.
	 *
	 * \param	other	The SocketSet to copy its value from.
	 */
	SocketSet(const SocketSet & other);

	/**
	 * Assigns the contents of other to this object.
	 *
	 * Because select modifies the set, the original data is lost.
	 * Now you can do the following:
	 *
	 * \code
	 * SocketSet master(sock);
	 * SocketSet work;
	 *
	 * while (! stop) {
	 * 	work = master;
	 *  if (SocketSet::select(work, NULL, NULL, mktv(1, 0)) > 0) {
	 *    sock.read(&buf, sizeof(buf));
	 *  }
	 * }
	 * \endcode
	 *
	 * This is less resource intensive than creating the object again
	 * and again using the copy constructor.
	 *
	 * \param	other	The SocketSet to copy its contents from.
	 */
	SocketSet & operator=(const SocketSet & other);


	/**
	 * Zero's this set.
	 */
	void	zero();

	/**
	 * Adds a Socket to this set.
	 */
	void	set(Socket & sock);

	/**
	 * Checks if a socket is set in this set.
	 */
	bool	isSet(Socket & sock);


	/**
	 * Executes a select call on the specified sockets sets. The SocketSets's contents will be
	 * modified to only contain those sockets which are ready for the given operation.
	 *
	 * \param	read	Check which sockets are ready for read, or connected.
	 * \param	write	Check which sockets are ready for writing.
	 * \param	error	Check which sockets have an error.
	 * \param	timeout	Check for this period of time.
	 *
	 * \return	The number of sockets that have changed, may be 0.
	 */
	static	int	select(SocketSet * read, SocketSet * write, SocketSet * error, timeval & timeout);



	/**
	 * Die SocketSet! Die!
	 */
	virtual ~SocketSet();


private:
	fd_set	_fd_set;
	SOCKET	_fd_max;
};

END_NKF_NET

#endif /* SOCKETSET_H_ */
