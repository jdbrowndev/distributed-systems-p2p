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
#include "../graph_traversal.h"
#include "../globals.h"
#include "../service_request.h"
#include "client_interface.h"

namespace brown {
	client_interface::client_interface(char* port): port(port), connection((char*)"", (char*)""), neighborId(0) {
		commands["exit"] = "Exits the client interface but leaves the server running";
		commands["list"] = "Lists all neighbors known to this client or all nodes in the system (-s flag)";
		commands["select"] = "Selects a neighbor ID from the neighbors list for future communication";
		commands["file"] = "Queries the selected neighbor or system (-s flag) for a file";
		commands["share"] = "Shares up to 3 known neighbors with the selected neighbor";
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
		} else if(isList((char*)command.c_str())) {
			handleListCommand();
		} else if(isSelect((char*)command.c_str())) {
			handleSelectCommand();
		} else if(isFile((char*)command.c_str())) {
			handleFileCommand();
		} else if(isShare((char*)command.c_str())) {
			handleShareCommand();
		} else if(strcasecmp(command.c_str(), "exit") == 0) {
			resetConnection();
			std::cout << "Client exiting... the server will continue running until manually terminated"
					<< std::endl << std::endl;
		} else {
			std::cout << "Command '" << command << "' not recognized." << std::endl;
		}
	}

	void client_interface::handleListCommand() {
		char* cmd = strtok((char*)command.c_str(), " ");
		char* systemFlag = strtok(NULL, " ");
		
		if(!systemFlag) {  //client-based list command
			printVectorStrings(neighbors, "Neighbors", true);
		} else if(strncmp(systemFlag, "-s", 2) == 0) { //system-based list command
			runSystemQuery();
		} else { //incorrect flag, print usage
			printListUsage();
		}
	}
	
	void client_interface::handleSelectCommand() {
		char* cmd = strtok((char*)command.c_str(), " ");
		char* neighborIdArg = strtok(NULL, " ");
		int neighborIdTmp;

		if(neighborIdArg) {
			neighborIdTmp = atoi(neighborIdArg);
			if(!isNeighbor(neighborIdTmp)) {
				std::cout << "Client: The ID you provided is not assigned to a neighbor." << std::endl;
			} else {
				// clean up old connection
				resetConnection();
				// attempt connection
				neighborId = neighborIdTmp;
				instantiateConnection();
				if(connection.openConnection()) {
					runEntryQuery();
					runPingQuery();
				} else {
					neighborId = 0;
					std::cout << "Client: Could not connect to the neighbor you selected. "
							<< "Please select a different neighbor." << std::endl;
				}
			}
		} else {
			printSelectUsage();
		}
	}

	void client_interface::handleFileCommand() {
		char* cmd = strtok((char*)command.c_str(), " ");
		char* systemFlag = strtok(NULL, " ");
		char* fileNameTmp = strtok(NULL, " ");
		
		if(systemFlag) { //there is a second parameter
			if(strncmp(systemFlag, "-s", 2) == 0) {  //system wide flag is given
				if(fileNameTmp) {	//filename given, correct syntax
					strncpy(fileName, fileNameTmp, FILE_NAME_MAX_LENGTH);
					runSystemQuery(std::string(fileName));
				} else {	//filename not given, incorrect syntax
					printFileUsage();
				}
			} else if (!fileNameTmp && neighborId != 0) { //filename given, no system flag and a neighbor has been selected.
				strncpy(fileName, systemFlag, FILE_NAME_MAX_LENGTH);
				runLookupQuery();
			} else {  //second parameter is unrecognized with filename, incorrect syntax
				printFileUsage();
			}
		} else {
			printFileUsage();
		}
	}
	
	void client_interface::handleShareCommand() {
		char* cmd = strtok((char*)command.c_str(), " ");

		if(neighborId == 0) { // if neighbor-id has never been given
			printShareUsage();
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

	void client_interface::runEntryQuery() {
		connection.sendRequest(createServiceRequest(0, (char*)"", (char*)""));
	}

	void client_interface::runPingQuery() {
		connection.sendRequest(createServiceRequest(2, (char*)"ping", (char*)""));
	}
	
	void client_interface::runLookupQuery() {
		connection.sendRequest(createServiceRequest(2, (char*)"lookup", fileName));
	}

	void client_interface::runShareQuery() {
		connection.sendRequest(createServiceRequest(2, (char*)"ping", (char*)""));
		connection.sendRequest(createServiceRequest(4, (char*)"neighbors",
				(char*)encodeNeighbors(neighbors, MAX_NEIGHBORS_TO_SHARE).c_str()));
	}
	
	void client_interface::runSystemQuery() {
		runSystemQuery("");
	}

	void client_interface::runSystemQuery(std::string fileName) {
		graph_traversal traversal(atoi(port));
		// Put my host:port into the initial visited list
		char domainName[256];
		gethostname(domainName, sizeof(domainName));
		std::stringstream strStream;
		strStream << domainName << ":" << port;
		std::vector<std::string> initialVisited;
		initialVisited.push_back(strStream.str());
		// Traverse and get result
		graph_traversal_result result = traversal.traverse(initialVisited, fileName);
		// If the query is a system-wide ping, print the results
		if(fileName.compare("") == 0) {
			printVectorStrings(result.visited, "Network Nodes", false);
		// Else, if the query is system-wide file lookup, print not found if
	    // no file was found (note: if file was found, client_connection.cc will
		// print the contents automatically
		} else if(result.fileContents.length() == 0) {
			std::cout << "Client: Could not find content file \"" << fileName << "\" in the system"
					<< std::endl;
		}
	}

	void client_interface::runExitQuery() {
		connection.sendRequest(createServiceRequest(1, (char*)"", (char*)""));
	}

	void client_interface::instantiateConnection() {
		server = neighbors.at(neighborId-1);
		std::string neighbor = server.c_str(); // c_str() forces a deep copy
		char* host = new char[HOST_MAX_LENGTH + 1];
		char* port = new char[PORT_MAX_LENGTH + 1];
		strncpy(host, strtok((char*)neighbor.c_str(), ":"), HOST_MAX_LENGTH);
		strncpy(port, strtok(NULL, ":"), PORT_MAX_LENGTH);
		connection = client_connection(host, port);
	}

	bool client_interface::isList(char* str) {
		regex_t queryRegex;
		regcomp(&queryRegex, "^list.*", 0);
		return regexec(&queryRegex, str, 0, NULL, 0) == 0;
	}
	
	bool client_interface::isSelect(char* str) {
		regex_t queryRegex;
		regcomp(&queryRegex, "^select.*", 0);
		return regexec(&queryRegex, str, 0, NULL, 0) == 0;
	}
	
	bool client_interface::isFile(char* str) {
		regex_t queryRegex;
		regcomp(&queryRegex, "^file.*", 0);
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
		strncpy(request.visited, "", sizeof(request.visited));
		return request;
	}
	
	void client_interface::printListUsage() {
		std::cout << "Usage: list [-s]" << std::endl;
		std::cout << "   -s = flag to list all nodes in system." << std::endl;
		std::cout << "Without -s, only known neighbors will be printed." << std::endl;
	}
	
	void client_interface::printSelectUsage() {
		std::cout << "Usage: select neighbor-id" << std::endl;
		std::cout << "   neighbor-id = ID of a neighbor to select (see 'list' command)." << std::endl;
		std::cout << "A ping request will be sent to the neighbor associated with the selected ID." << std::endl;
	}
	
	void client_interface::printFileUsage() {
		std::cout << "Usage: file [-s] filename" << std::endl;
		std::cout << "   -s = flag to search all nodes in system." << std::endl;
		std::cout << "   filename = File to search for." << std::endl;
		std::cout << "File search will default to the selected neighbor if -s is not given." << std::endl;
	}

	void client_interface::printShareUsage() {
		std::cout << "Usage: share" << std::endl;
		std::cout << "Share request defaults to a selected neighbor." << std::endl;
		std::cout << "A neighbor must be selected for the command to work correctly." << std::endl;
	}

	void client_interface::resetConnection() {
		if(neighborId != 0) {
			std::cout << "Client: Closing previous connection with " << server << " ..." << std::endl;
			runExitQuery();
			connection.closeConnection();
		}
		server = "";
	}
}
