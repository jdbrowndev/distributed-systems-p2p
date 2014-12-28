//
// network_connection.cc
//
// Author: Jordan Brown
// Date: Sep 27, 2014
//
// Parent class for TCP/IP connections

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include "network_connection.h"

namespace brown {
    network_connection::network_connection() {
        socketdesc = 0;
    }

    int network_connection::getSocketDesc() {
        return socketdesc;
    }

    void network_connection::openSocket() {
        socketdesc = socket(AF_INET, SOCK_STREAM, 0);
        if (socketdesc < 0) {
            std::cout << "Network: Error creating socket" << std::endl;
            exit(0);
        }
    }

    bool network_connection::setAddressInfo(char* host, char* port) {
        if (getaddrinfo(host, port, NULL, &myinfo) != 0) {
            std::cout << "Network: Error getting address" << std::endl;
            return false;
        }
        return true;
    }
}
