//
// client_connection.cc
//
// Author: Jordan Brown
// Date: Sep 3, 2014
//
// Connects client to server application

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include "../network_connection.h"
#include "../service_request.h"
#include "client_connection.h"

namespace brown {
	client_connection::client_connection(char* host, char* port):host(host),
			port(port) { }

	void client_connection::openConnection() {
		openSocket();
		setAddressInfo(host, port);

		// Connect to the host
		connection = connect(socketdesc, myinfo->ai_addr, myinfo->ai_addrlen);
		if (connection < 0) {
			std::cout << "Client: Error in connect" << std::endl;
		}
	}

	void client_connection::closeConnection() {
		close(socketdesc);
	}

	service_request client_connection::sendRequest(service_request request) {
		// Send request
		write(socketdesc, (char*)&request, sizeof(service_request));
		std::cout << "Client: Sent request to " << host << ":" << port << std::endl;

		// Read response
		service_request response;
		read(socketdesc, (char*)&response, sizeof(service_request));
		std::cout << "Client: Received response from " << response.domainName << ":"
				<< response.portNumber << std::endl;

		return response;
	}
}
