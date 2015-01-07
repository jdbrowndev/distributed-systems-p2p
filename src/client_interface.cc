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
#include "graph_traversal.h"
#include "globals.h"
#include "service_request.h"
#include "client_interface.h"

namespace brown {
    client_interface::client_interface(std::string port): port(port), connection("", ""), neighborId(0) {
        commands["exit"] = "Exits the client interface but leaves the server running";
        commands["list"] = "Lists all neighbors known to this client or all nodes in the system (-s flag)";
        commands["select"] = "Selects a neighbor ID from the neighbors list for future communication";
        commands["file"] = "Queries the selected neighbor or system (-s flag) for a file";
        commands["share"] = "Shares up to 3 known neighbors with the selected neighbor";
    }

    void client_interface::initialize() {
        printHelper.printWelcomeMessage();
        if(neighbors.size() == 0) {
            promptForNeighbor();
        }
        do {
            promptCommand();
            parseCommand();
        } while (strcasecmp(command.c_str(), "exit") != 0);
    }

    void client_interface::promptForNeighbor() {
        std::cout << "\nYou have no neighbors. Please provide a host and port of a neighbor."
            << std::endl << "host>";
        std::string host, port;
        getline(std::cin, host);
        std::cout << "port>";
        getline(std::cin, port);
        std::string neighbor = host + ":" + port;
        neighbors.append(neighbor);
        fileManager.appendNeighbor(neighbor);
    }

    void client_interface::promptCommand() {
        std::cout << std::endl << "client> ";
        getline(std::cin, command);
    }

    void client_interface::parseCommand() {
        if(strcasecmp(command.c_str(), "help") == 0) {
            printHelper.printCommands(commands);
        } else if(isList(command)) {
            handleListCommand();
        } else if(isSelect(command)) {
            handleSelectCommand();
        } else if(isFile(command)) {
            handleFileCommand();
        } else if(isShare(command)) {
            handleShareCommand();
        } else if(strcasecmp(command.c_str(), "exit") == 0) {
            resetConnection();
            std::cout << "Client exiting... the server will continue running until manually terminated\n"
                << std::endl;
        } else {
            std::cout << "Command '" << command << "' not recognized." << std::endl;
        }
    }

    void client_interface::handleListCommand() {
        std::stringstream tokenizer(command);
        std::string cmd, arg1;
        getline(tokenizer, cmd, ' ');
        getline(tokenizer, arg1, ' ');

        if(arg1.length() == 0) {  // client-based list command
            neighbors.print();
        } else if(strncmp(arg1.c_str(), "-s", 2) == 0) { // system-based list command
            runSystemQuery();
        } else { // incorrect flag, print usage
            printHelper.printListUsage();
        }
    }

    void client_interface::handleSelectCommand() {
        std::stringstream tokenizer(command);
        std::string cmd, neighborIdArg;
        getline(tokenizer, cmd, ' ');
        getline(tokenizer, neighborIdArg, ' ');

        if(neighborIdArg.length() > 0) {
            int neighborIdArgInt;
            neighborIdArgInt = atoi(neighborIdArg.c_str());
            if(!isNeighbor(neighborIdArgInt)) {
                std::cout << "Client: The ID you provided is not assigned to a neighbor." << std::endl;
            } else {
                // clean up old connection
                resetConnection();
                // attempt connection
                neighborId = neighborIdArgInt;
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
            printHelper.printSelectUsage();
        }
    }

    void client_interface::handleFileCommand() {
        std::stringstream tokenizer(command);
        std::string cmd, arg1, arg2;
        getline(tokenizer, cmd, ' ');
        getline(tokenizer, arg1, ' ');
        getline(tokenizer, arg2, ' ');

        if(arg1.length() > 0) { // one arg minimum is required
            if(strncmp(arg1.c_str(), "-s", 2) == 0) { // system wide flag is given
                if(arg2.length() > 0) { // filename given, correct syntax
                    runSystemQuery(arg2);
                } else { // filename not given
                    printHelper.printFileUsage();
                }
            } else if (arg2.length() == 0 && neighborId > 0) { 
                // arg1 is the filename (no arg2) AND a neighbor has been selected.
                runLookupQuery(arg1);
            } else { // bad syntax and/or no neighbor selected 
                printHelper.printFileUsage();
            }
        } else {
            printHelper.printFileUsage();
        }
    }

    void client_interface::handleShareCommand() {
        if(neighborId > 0) {
            runShareQuery();
        } else {
            printHelper.printShareUsage();
        }
    }

    void client_interface::runEntryQuery() {
        connection.sendRequest(createServiceRequest(atoi(port.c_str()), 0));
    }

    void client_interface::runPingQuery() {
        connection.sendRequest(createServiceRequest(atoi(port.c_str()), 2, "ping"));
    }

    void client_interface::runLookupQuery(std::string fileName) {
        connection.sendRequest(createServiceRequest(atoi(port.c_str()), 2, "lookup", fileName));
    }

    void client_interface::runShareQuery() {
        connection.sendRequest(createServiceRequest(atoi(port.c_str()), 2, "ping"));
        std::vector<std::string> neighborsCopy;
        neighbors.copy(neighborsCopy);
        connection.sendRequest(createServiceRequest(atoi(port.c_str()), 4, "neighbors",
                    serializer.encodeNeighbors(neighborsCopy, MAX_NEIGHBORS_TO_SHARE)));
    }

    void client_interface::runSystemQuery() {
        runSystemQuery("");
    }

    void client_interface::runSystemQuery(std::string fileName) {
        graph_traversal traversal(atoi(port.c_str()));
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
            printHelper.printDecoratedTitle("Network Nodes");
            for(std::vector<std::string>::iterator it = result.visited.begin(); it != result.visited.end();
                    ++it) {
                std::cout << *it << std::endl;
            }
            // Else, if the query is system-wide file lookup, print not found if
            // no file was found (note: if file was found, client_connection.cc will
            // print the contents automatically
        } else if(result.fileContents.length() == 0) {
            std::cout << "Client: Could not find content file \"" << fileName << "\" in the system"
                << std::endl;
        }
    }

    void client_interface::runExitQuery() {
        connection.sendRequest(createServiceRequest(atoi(port.c_str()), 1));
    }

    void client_interface::instantiateConnection() {
        neighbor_serializer::host_port_tokens tokens = serializer.splitNeighbor(neighbors.at(neighborId-1));
        server = tokens.host + ":" + tokens.port;
        connection = client_connection(tokens.host, tokens.port);
    }

    bool client_interface::isList(std::string str) {
        regex_t queryRegex;
        regcomp(&queryRegex, "^list.*", 0);
        return regexec(&queryRegex, &str[0], 0, NULL, 0) == 0;
    }

    bool client_interface::isSelect(std::string str) {
        regex_t queryRegex;
        regcomp(&queryRegex, "^select.*", 0);
        return regexec(&queryRegex, &str[0], 0, NULL, 0) == 0;
    }

    bool client_interface::isFile(std::string str) {
        regex_t queryRegex;
        regcomp(&queryRegex, "^file.*", 0);
        return regexec(&queryRegex, &str[0], 0, NULL, 0) == 0;
    }

    bool client_interface::isShare(std::string str) {
        regex_t queryRegex;
        regcomp(&queryRegex, "^share.*", 0);
        return regexec(&queryRegex, &str[0], 0, NULL, 0) == 0;
    }

    bool client_interface::isNeighbor(int id) {
        return id > 0 && id <= neighbors.size();
    }

    void client_interface::resetConnection() {
        if(neighborId > 0) {
            std::cout << "Client: Closing previous connection with " << server << " ..." << std::endl;
            runExitQuery();
            connection.closeConnection();
        }
        server = "";
    }
}
