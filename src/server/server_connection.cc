//
// server_connection.cc
//
// Author: Jordan Brown
// Date: Sep 27, 2014
//
//

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include "../network_connection.h"
#include "../globals.h"
#include "server_connection.h"

namespace brown {
	server_connection::server_connection(int lowPort, int highPort):
			network_connection(), lowPort(lowPort), highPort(highPort) {
		port = new char[PORT_MAX_LENGTH+1];
	}

	void server_connection::openConnection() {
		openSocket();

		// Bind socket to an address
		if (!stepToPort()) {
			std::cout << "Server: Error binding to socket" << std::endl;
			exit(0);
		}

		// Listen to the socket
		if (listen(socketdesc, 1) < 0) {
			std::cout << "Server: Error in listen" << std::endl;
			exit(0);
		}

		std::cout << "Server: Listening on port " << port << std::endl;
	}

	bool server_connection::stepToPort() {
		int current = lowPort;
		while (current <= highPort) {
			char currentStr[PORT_MAX_LENGTH + 1] = "";
			sprintf(currentStr, "%d", current);
			setAddressInfo((char*)"0.0.0.0", currentStr);
			if (bind(socketdesc, myinfo->ai_addr, myinfo->ai_addrlen) >= 0) {
				strncpy(port, currentStr, PORT_MAX_LENGTH); // this copy is safe because port is 1 character longer
				return true;
			}
			current++;
		}
		return false;
	}

	char* server_connection::getPort() {
		return port;
	}
}
