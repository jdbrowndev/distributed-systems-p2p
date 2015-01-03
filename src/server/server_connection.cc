//
// server_connection.cc
//
// Author: Jordan Brown
// Date: Sep 27, 2014
//
// Establishes connection to receive requests from other peers

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include "../network_connection.h"
#include "../globals.h"
#include "server_connection.h"

namespace brown {
    server_connection::server_connection(int lowPort, int highPort):
            network_connection(), lowPort(lowPort), highPort(highPort) { }

    void server_connection::openConnection() {
        if (!openSocket()) {
            exit(0); 
        }

        // Bind socket to an address
        if (!stepToPort()) {
            std::cout << "Server: Error binding to socket" << std::endl;
            exit(0);
        }

        // Listen to the socket
        if (listen(socketdesc, 1) < 0) {
            std::cout << "Server: Error in listen" << std::endl;
            exit(0);
        }

        std::cout << "Server: Listening on port " << port << std::endl;
    }

    bool server_connection::stepToPort() {
        char address[] = "0.0.0.0";
        int current = lowPort;
        while (current <= highPort) {
            char currentStr[PORT_MAX_LENGTH + 1] = "";
            sprintf(currentStr, "%d", current);
            setAddressInfo(address, currentStr);
            if (bind(socketdesc, myinfo->ai_addr, myinfo->ai_addrlen) >= 0) {
                port = std::string(currentStr);
                return true;
            }
            current++;
        }
        return false;
    }
}
