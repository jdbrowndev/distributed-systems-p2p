//
// client_interface.h
//
// Author: Jordan Brown
// Date: Sep 28, 2014
//
// Manages client command line interface

#ifndef CLIENT_INTERFACE_H_
#define CLIENT_INTERFACE_H_

#include <map>
#include <string>
#include "../service_request.h"

namespace brown {
	class client_interface {
	private:
		const static int COMMAND_LIST_INDENT = 3;
		const static int COMMAND_LIST_SPACING = 8;
		const static int NEIGHBORS_LIST_INDENT = 3;
		char* port;
		std::string command;
		std::map<std::string, std::string> commands;
		void promptForNeighbor();
		void promptCommand();
		void parseCommand();
		void printWelcomeMessage();
		void printCommands();
		void printNeighbors();
		void runQuery(char* args);
		bool isQuery(char* str);
		bool isValidHostAndPort(char* hostAndPort);
		bool isNeighbor(char* neighbor);
		service_request createServiceRequest(int requestType);
	public:
		client_interface(char* port);
		void initialize();
	};
}

#endif /* CLIENT_INTERFACE_H_ */
