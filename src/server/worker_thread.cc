//
// worker_thread.cc
//
// Author: Jordan Brown
// Date: Sep 6, 2014
//
// Handles connection reads/writes

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include "worker_thread.h"
#include "../globals.h"
#include "../service_request.h"

namespace brown {
	worker_thread::worker_thread(int connection, int port):connection(connection), port(port), exit(false) { }

	void worker_thread::handleRequests() {
		while(!exit) {
			readRequest();
			writeResponse();
		}
		close(connection);
	}

	void worker_thread::readRequest() {
		read(connection, (char*)&request, sizeof(service_request));
		std::stringstream strStream;
		strStream << request.domainName << ":" << request.portNumber;
		std::string client = strStream.str();
		strStream.str(std::string()); // clear stream
		switch(request.requestType) {
			case 0:
				// new client TODO: check if client is already in the list
				pthread_mutex_lock(&connectedClientsMutex);
					connectedClients.push_back(client);
					strStream << "Server: Received request from new client " << client
							<< " [" << connectedClients.size() << " client(s) connected]" << std::endl;
				pthread_mutex_unlock(&connectedClientsMutex);
				break;
			case 1:
				// client exiting TODO: do not remove client from the list
				pthread_mutex_lock(&connectedClientsMutex);
					connectedClients.erase(std::remove(connectedClients.begin(),
							connectedClients.end(), client), connectedClients.end());
					strStream << "Server: Received exit request from client " << client
							<< " [" << connectedClients.size() << " client(s) connected]" << std::endl;
				pthread_mutex_unlock(&connectedClientsMutex);
				exit = true;
				break;
			case 2:
				// client query
				strStream << "Server: Received query request from " << client << std::endl;
				break;
		}
		std::cout << strStream.str();
	}

	void worker_thread::writeResponse() {
		service_request response;
		strncpy(response.domainName, DOMAIN_NAME, 256);
		response.portNumber = port;
		response.requestType = 3;
		strncpy(response.requestString, "", 1);
		response.requestId = 0;
		strncpy(response.payload, "", 1);
		write(connection, (char*)&response, sizeof(service_request));
		std::cout << "Server: Response sent to " << request.domainName << ":" << request.portNumber << std::endl;
	}
}
