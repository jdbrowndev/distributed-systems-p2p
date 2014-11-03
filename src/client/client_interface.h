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
#include "client_connection.h"
#include "../service_request.h"

namespace brown {
	class client_interface {
	private:
		const static int COMMAND_LIST_INDENT = 3;
		const static int COMMAND_LIST_SPACING = 8;
		const static int FILE_NAME_MAX_LENGTH = 12;
		char* port;
		std::string command;
		int neighborId;
		client_connection* connection;
		std::string server;
		char fileName[FILE_NAME_MAX_LENGTH+1];
		std::map<std::string, std::string> commands;
		void promptForNeighbor();
		void promptCommand();
		void parseCommand();
		void printWelcomeMessage();
		void printCommands();
		void handleQueryCommand();
		void handleShareCommand();
		void runPingQuery();
		void runLookupQuery();
		void runShareQuery();
		bool isQuery(char* str);
		bool isShare(char* str);
		void instantiateConnection();
		bool isNeighbor(int neighborID);
		service_request createServiceRequest(int requestType, char* requestString, char* payload);
		std::string createSharePayload();
		void printQueryUsage();
		void printShareUsage();
		void resetVariables();
	public:
		client_interface(char* port);
		void initialize();
	};
}

#endif /* CLIENT_INTERFACE_H_ */
