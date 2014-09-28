//
// server_connection.h
//
// Author: Jordan Brown
// Date: Sep 27, 2014
//
//
#ifndef SERVER_CONNECTION_H_
#define SERVER_CONNECTION_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "../network_connection.h"

namespace brown {
	class server_connection : public network_connection {
	private:
		int lowPort;
		int highPort;
		char* port;
		bool stepToPort();
	public:
		server_connection(int lowPort, int highPort);
		char* openConnection();
	};
}

#endif /* SERVER_CONNECTION_H_ */
