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
			read(connection, (char*)&request, sizeof(service_request));
			if(client.length() == 0) { buildClientString(); }
			handleRequest();
		}
		close(connection);
	}

	void worker_thread::handleRequest() {
		switch(request.requestType) {
			case 0:
				handleNewClientRequest();
				break;
			case 1:
				handleClientExitRequest();
				break;
			case 2:
				handleClientQueryRequest();
				break;
			case 4:
				handleClientShareRequest();
				break;
		}
	}

	void worker_thread::handleNewClientRequest() {
		std::cout << "Server: Received request from new client " << client << std::endl;
		writeResponse((char*)"", (char*)"");
	}

	void worker_thread::handleClientExitRequest() {
		tryAppendNeighbor(client);
		std::cout << "Server: Received exit request from client " << client << std::endl;
		exit = true;
		writeResponse((char*)"", (char*)"");
	}

	void worker_thread::handleClientQueryRequest() {
		if(strcasecmp(request.requestString, "ping") == 0) {
			std::cout << "Server: Received ping request from client " << client
					<< ". Responding with \"alive\"" << std::endl;
			writeResponse((char*)"alive", (char*)"");
		} else if(strcasecmp(request.requestString, "lookup") == 0) {
			std::cout << "Server: Received request for content file \"" << request.payload
					<< "\" from client " << client << std::endl;
			std::string fileContents = fileManager.readContentFile(std::string(request.payload));
			if(fileContents.length() > 0) {
				std::cout << "Server: Found content file \"" << request.payload
						<< "\" for client " << client << std::endl;
				writeResponse((char*)"found", (char*)fileContents.c_str());
			} else {
				std::cout << "Server: Could not find content file \"" << request.payload
						<< "\" for client " << client << std::endl;
				writeResponse((char*)"not found", (char*)"");
			}
		} else {
			std::cout << "Server: Received query request from " << client
					<< ". Sending empty response." << std::endl;
			writeResponse((char*)"", (char*)"");
		}
	}

	void worker_thread::handleClientShareRequest() {
		if(strcasecmp(request.requestString, "neighbors") == 0) {
			std::cout << "Server: Received neighbor share request from client " << client << std::endl;
			appendSharedNeighbors();
			printNeighbors();
			writeResponse((char*)"thanks", (char*)"");
		} else {
			std::cout << "Server: Received neighbor share request from client " << client
					<< ", but it is missing some information. Responding with \"error\"" << std::endl;
			writeResponse((char*)"", (char*)"");
		}
	}

	void worker_thread::writeResponse(char* requestString, char* payload) {
		service_request response;
		gethostname(response.domainName, sizeof(response.domainName));
		response.portNumber = port;
		response.requestType = 3;
		strcpy(response.requestString, requestString);
		response.requestId = 0;
		strcpy(response.payload, payload);
		write(connection, (char*)&response, sizeof(service_request));
		std::cout << "Server: Response sent to " << client << std::endl;
	}

	void worker_thread::buildClientString() {
		std::stringstream strStream;
		strStream << request.domainName << ":" << request.portNumber;
		client = strStream.str();
	}

	void worker_thread::appendSharedNeighbors() {
		int neighborPairCount = atoi(strtok(request.payload, ";"));
		std::stringstream strStream;
		for(int i = 1; i <= neighborPairCount; i++) {
			char* host = strtok(NULL, ";");
			char* port = strtok(NULL, ";");
			strStream << host << ":" << port;
			std::string neighbor = strStream.str();
			if(!tryAppendNeighbor(neighbor)) {
				std::cout << "Server: Cannot add " << neighbor << " to neighbors list: "
						<< neighbor << " is already in the list" << std::endl;
			}
			strStream.str("");
		}
	}

	bool worker_thread::tryAppendNeighbor(std::string neighbor) {
		if(std::find(neighbors.begin(), neighbors.end(), neighbor) == neighbors.end()) {
			appendToNeighborsVector(neighbor);
			appendToNeighborsFile(neighbor);
			return true;
		}
		return false;
	}
}
