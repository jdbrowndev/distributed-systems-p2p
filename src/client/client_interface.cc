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
	client_interface::client_interface(char* port): port(port), connection(NULL), neighborId(0) {
		commands["exit"] = "Exits the client interface but leaves the server running";
		commands["list"] = "Lists all known neighbors";
		commands["query"] = "Queries a neighbor";
		commands["share"] = "Shares up to 3 known neighbors with a neighbor";
	}

	void client_interface::initialize() {
		printWelcomeMessage();
		if(neighbors.empty()) {
			promptForNeighbor();
		}
		do {
			promptCommand();
			parseCommand();
			resetVariables();
		} while (strcasecmp(command.c_str(), "exit") != 0);
	}

	void client_interface::printWelcomeMessage() {
		std::cout << "Client: CLI launched. Type 'help' for a list of commands." << std::endl;
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
			handleQueryCommand();
		} else if(isShare((char*)command.c_str())) {
			handleShareCommand();
		} else if(strcasecmp(command.c_str(), "exit") == 0) {
			std::cout << "Client exiting... the server will continue running until manually terminated"
					<< std::endl << std::endl;
		} else {
			std::cout << "Command '" << command << "' not recognized." << std::endl;
		}
	}

	void client_interface::handleQueryCommand() {
		char* cmd = strtok((char*)command.c_str(), " ");
		char* neighborIdArg = strtok(NULL, " ");
		char* fileNameTmp = strtok(NULL, " "); // TODO: handle case where file name is given, but neighbor id is not

		if(neighborIdArg) { // old neighbor-id will be preserved if argument not given
			neighborId = atoi(neighborIdArg);
		}
		if(!neighborIdArg && neighborId == 0) { // if neighbor-id has never been given
			printQueryUsage();
		} else if(!isNeighbor(neighborId)) {
			std::cout << "Client: The ID you provided is not assigned to a neighbor." << std::endl;
		} else if(fileNameTmp && (int)strlen(fileNameTmp) > 12) {
			printQueryUsage();
		} else if(fileNameTmp) {
			strncpy(fileName, fileNameTmp, FILE_NAME_MAX_LENGTH);
			runLookupQuery();
		} else {
			runPingQuery();
		}
	}

	void client_interface::handleShareCommand() {
		char* cmd = strtok((char*)command.c_str(), " ");
		char* neighborIdArg = strtok(NULL, " ");

		if(neighborIdArg) { // old neighbor-id will be preserved if argument not given
			neighborId = atoi(neighborIdArg);
		}
		if(!neighborIdArg && neighborId == 0) { // if neighbor-id has never been given
			printShareUsage();
		} else if(!isNeighbor(neighborId)) {
			std::cout << "Client: The ID you provided is not assigned to a neighbor." << std::endl;
		} else {
			runShareQuery();
		}
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

	void client_interface::runPingQuery() {
		instantiateConnection();
		client_connection myConnection = *connection;
		if(myConnection.openConnection()) {
			myConnection.sendRequest(createServiceRequest(0, (char*)"", (char*)""));
			myConnection.sendRequest(createServiceRequest(2, (char*)"ping", (char*)""));
			myConnection.sendRequest(createServiceRequest(1, (char*)"", (char*)""));
			myConnection.closeConnection();
		}
	}
	
	void client_interface::runLookupQuery() {
		instantiateConnection();
		client_connection myConnection = *connection;
		if(myConnection.openConnection()) {
			myConnection.sendRequest(createServiceRequest(0, (char*)"", (char*)""));
			myConnection.sendRequest(createServiceRequest(2, (char*)"lookup", fileName));
			myConnection.sendRequest(createServiceRequest(1, (char*)"", (char*)""));
			myConnection.closeConnection();
		}
	}

	void client_interface::runShareQuery() {
		instantiateConnection();
		client_connection myConnection = *connection;
		if(myConnection.openConnection()) {
			myConnection.sendRequest(createServiceRequest(0, (char*)"", (char*)""));
			myConnection.sendRequest(createServiceRequest(2, (char*)"ping", (char*)""));
			myConnection.sendRequest(createServiceRequest(4, (char*)"neighbors",
					(char*)createSharePayload().c_str()));
			printNeighbors();
			myConnection.sendRequest(createServiceRequest(1, (char*)"", (char*)""));
			myConnection.closeConnection();
		}
	}
	
	void client_interface::instantiateConnection() {
		server = neighbors.at(neighborId-1);
		std::string neighbor = server.c_str(); // c_str() forces a deep copy
		char* host = new char[HOST_MAX_LENGTH + 1];
		char* port = new char[PORT_MAX_LENGTH + 1];
		strncpy(host, strtok((char*)neighbor.c_str(), ":"), HOST_MAX_LENGTH);
		strncpy(port, strtok(NULL, ":"), PORT_MAX_LENGTH);
		connection = new client_connection(host, port);
	}

	bool client_interface::isQuery(char* str) {
		regex_t queryRegex;
		regcomp(&queryRegex, "^query.*", 0);
		return regexec(&queryRegex, str, 0, NULL, 0) == 0;
	}
	
	bool client_interface::isShare(char* str) {
		regex_t queryRegex;
		regcomp(&queryRegex, "^share.*", 0);
		return regexec(&queryRegex, str, 0, NULL, 0) == 0;
	}

	bool client_interface::isNeighbor(int id) {
		return id > 0 && id <= neighbors.size();
	}

	service_request client_interface::createServiceRequest(int requestType, char* requestString, char* payload) {
		service_request request;
		gethostname(request.domainName, sizeof(request.domainName));
		request.portNumber = atoi(port);
		request.requestId = 0;
		request.requestType = requestType;
		strncpy(request.requestString, requestString, sizeof(request.requestString));
		strncpy(request.payload, payload, sizeof(request.payload));
		return request;
	}
	
	std::string client_interface::createSharePayload() {
		std::stringstream payloadBuilder;
		std::string strNeighbor;
		std::string separator = ";";
		int payloadSize;
		payloadSize = neighbors.size() >= 3 ? 3 : neighbors.size();
		payloadBuilder << payloadSize;
		for (int i = 0; i < payloadSize; i++) {
			payloadBuilder << separator;
			strNeighbor = neighbors.at(i).c_str();
			payloadBuilder << strtok((char*)strNeighbor.c_str(), ":");
			payloadBuilder << separator;
			payloadBuilder << strtok(NULL, ":");
		}
		std::string payload = payloadBuilder.str();
		std::cout << "Client: Preparing to share neighbors: "
				<< payload << std::endl;
		return payload;
	}

	void client_interface::printQueryUsage() {
		std::cout << "Usage: query [neighbor-id] [file]" << std::endl;
		std::cout << "   neighbor-id = ID of a neighbor to query (see 'list' command)."
				<< " The most recent ID will be used if this parameter is omitted."
				<< std::endl;
		std::cout << "   file = File to request from neighbor."
				<< " A ping request will be sent if this parameter is omitted." << std::endl;
	}

	void client_interface::printShareUsage() {
		std::cout << "Usage: share [neighbor-id]" << std::endl;
		std::cout << "   neighbor-id = ID of a neighbor to share neighbors with (see 'list' command)."
				<< " The most recent ID will be used if this parameter is omitted."
				<< std::endl;
	}

	void client_interface::resetVariables() {
		strncpy(fileName, "", FILE_NAME_MAX_LENGTH);
		if(connection) {
			delete connection;
			connection = NULL;
		}
		server = "";
	}
}
