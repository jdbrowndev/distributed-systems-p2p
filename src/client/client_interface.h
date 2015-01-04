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
#include "../neighbor_serializer.h"
#include "../print_helper.h"
#include "../service_request.h"

namespace brown {
    class client_interface {
    private:
        const static int MAX_NEIGHBORS_TO_SHARE = 3;
        std::string port; // The port this node is currently listening on
        std::string command;
        int neighborId;
        client_connection connection;
        std::string server;
        std::map<std::string, std::string> commands;
        neighbor_serializer serializer;
        print_helper printHelper;
        void promptForNeighbor();
        void promptCommand();
        void parseCommand();
        void handleListCommand();
        void handleSelectCommand();
        void handleFileCommand();
        void handleShareCommand();
        void runEntryQuery();
        void runPingQuery();
        void runLookupQuery(std::string fileName);
        void runShareQuery();
        void runSystemQuery();
        void runExitQuery();
        void runSystemQuery(std::string fileName);
        bool isList(std::string str);
        bool isSelect(std::string str);
        bool isFile(std::string str);
        bool isShare(std::string str);
        void instantiateConnection();
        bool isNeighbor(int neighborID);
        void resetConnection();
    public:
        client_interface(std::string port);
        void initialize();
    };
}

#endif /* CLIENT_INTERFACE_H_ */
