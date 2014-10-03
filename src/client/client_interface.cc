//
// client_interface.cc
//
// Author: Jordan Brown
// Date: Sep 28, 2014
//
// Manages client command line interface

#include <iostream>
#include <string.h>
#include <string>
#include <map>
#include <sstream>
#include <pthread.h>
#include <algorithm>
#include <regex.h>
#include "client_connection.h"
#include "../globals.h"
#include "../service_request.h"
#include "client_interface.h"

namespace brown {
	client_interface::client_interface(char* port): port(port) {
		commands["exit"] = "Exits the client interface but leaves the server running";
		commands["list"] = "Lists all neighbors available to query";
		commands["query"] = "Queries a neighbor";
	}

	void client_interface::initialize() {
		printWelcomeMessage();
		if(neighbors.empty()) {
			promptForNeighbor();
		}
		do {
			promptCommand();
			parseCommand();
		} while (strcasecmp(command.c_str(), "exit") != 0);
	}

	void client_interface::promptForNeighbor() {
		std::cout << std::endl << "You have no neighbors. Please provide a host and port of a neighbor."
				<< std::endl << "host>";
		std::string host, port;
		getline(std::cin, host);
		std::cout << "port>";
		getline(std::cin, port);
		std::string neighbor = host + ":" + port;
		appendToNeighborsVector(neighbor);
		appendToNeighborsFile(neighbor);
	}

	void client_interface::promptCommand() {
		std::cout << std::endl << "client> ";
		getline(std::cin, command);
	}

	void client_interface::parseCommand() {
		if(strcasecmp(command.c_str(), "help") == 0) {
			printCommands();
		} else if(strcasecmp(command.c_str(), "list") == 0) {
			printNeighbors();
		} else if(isQuery((char*)command.c_str())) {
			char* cmd = strtok((char*)command.c_str(), " ");
			char* args = strtok(NULL, " ");
			if(args == NULL || !isValidHostAndPort(args)) {
				std::cout << "Usage: query <host>:<port>" << std::endl;
			} else if(!isNeighbor(args)) {
				std::cout << "Client: The host and port you provided is not a neighbor." << std::endl;
			} else {
				runQuery(args);
			}
		} else if(strcasecmp(command.c_str(), "exit") == 0) {
			std::cout << "Client exiting... the server will continue running until manually terminated."
					<< std::endl << std::endl;
		} else {
			std::cout << "Command '" << command << "' not recognized." << std::endl;
		}
	}

	void client_interface::printWelcomeMessage() {
		std::cout << "Client: CLI launched. Type 'help' for a list of commands." << std::endl;
	}

	void client_interface::printCommands() {
		std::stringstream strStream;
		strStream << "Commands:" << std::endl;
		for(std::map<std::string, std::string>::iterator i = commands.begin(); i != commands.end(); i++) {
			for(int spaces = 1; spaces <= COMMAND_LIST_INDENT; spaces++) {
				strStream << " ";
			}
			strStream << i->first;
			for(int spaces = 1; spaces <= COMMAND_LIST_SPACING - i->first.length(); spaces++) {
				strStream << " ";
			}
			strStream << i->second << std::endl;
		}
		std::cout << strStream.str();
	}

	void client_interface::printNeighbors() {
		std::stringstream strStream;
		strStream << "Neighbors: " << std::endl;
		for(std::vector<std::string>::iterator i = neighbors.begin(); i != neighbors.end(); i++) {
			for(int spaces = 1; spaces <= NEIGHBORS_LIST_INDENT; spaces++) {
				strStream << " ";
			}
			strStream << *i << std::endl;
		}
		std::cout << strStream.str();
	}

	void client_interface::runQuery(char* args) {
		char* host = strtok(args, ":");
		char* port = strtok(NULL, ":");
		client_connection connection(host, port);
		connection.openConnection();
		connection.sendRequest(createServiceRequest(0));
		connection.sendRequest(createServiceRequest(2));
		connection.sendRequest(createServiceRequest(1));
		connection.closeConnection();
	}

	bool client_interface::isQuery(char* str) {
		regex_t queryRegex;
		regcomp(&queryRegex, "^query.*", 0);
		return regexec(&queryRegex, str, 0, NULL, 0) == 0;
	}

	bool client_interface::isValidHostAndPort(char* hostAndPort) {
		regex_t hostAndPortRegex;
		regcomp(&hostAndPortRegex, "^[-a-zA-Z0-9\\/\\.]+:[0-9]+$", REG_EXTENDED);
		return regexec(&hostAndPortRegex, hostAndPort, 0, NULL, 0) == 0;
	}

	bool client_interface::isNeighbor(char* neighbor) {
		return std::find(neighbors.begin(), neighbors.end(), std::string(neighbor)) != neighbors.end();
	}

	service_request client_interface::createServiceRequest(int requestType) {
		service_request request;
		gethostname(request.domainName, sizeof(request.domainName));
		request.portNumber = atoi(port);
		strncpy(request.requestString, "", 1);
		request.requestId = 0;
		strncpy(request.payload, "", 1);
		request.requestType = requestType;
		return request;
	}
}
