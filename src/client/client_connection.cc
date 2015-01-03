//
// client_connection.cc
//
// Author: Jordan Brown
// Date: Sep 3, 2014
//
// Connects to a peer

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include "../network_connection.h"
#include "../neighbor_serializer.h"
#include "../service_request.h"
#include "client_connection.h"

namespace brown {
    client_connection::client_connection(std::string host, std::string port):host(host),
            port(port) { }

    bool client_connection::openConnection() {
        if(openSocket() && setAddressInfo(host, port)) {
            std::cout << "Client: Connecting to " << host << ":" << port
                    << " ..." << std::endl;
            connection = connect(socketdesc, myinfo->ai_addr, myinfo->ai_addrlen);
            if (connection < 0) {
                std::cout << "Client: Error in connect" << std::endl;
            }
            return connection >= 0;
        }
        return false;
    }

    void client_connection::closeConnection() {
        close(socketdesc);
    }

    service_request client_connection::sendRequest(service_request request) {
        // Send request
        write(socketdesc, (char*)&request, sizeof(service_request));
        printRequestMessage(request);

        // Read response
        service_request response;
        read(socketdesc, (char*)&response, sizeof(service_request));
        printResponseMessage(response);

        return response;
    }

    void client_connection::printRequestMessage(service_request request) {
        switch(request.requestType) {
            case 0:
                std::cout << "Client: Sent entry request (type 0) to "
                    << host << ":" << port << std::endl;
                break;
            case 1:
                std::cout << "Client: Sent exit request (type 1) to " << host << ":"
                    << port << std::endl;
                break;
            case 2:
                if(strcasecmp(request.requestString, "ping") == 0) {
                    std::cout << "Client: Sent ping request (type 2) to "
                            << host << ":" << port << std::endl;
                } else if(strcasecmp(request.requestString, "lookup") == 0) {
                    std::cout << "Client: Sent lookup request (type 2) for content file \""
                            << request.payload << "\" to "
                            << host << ":" << port << std::endl;
                } else {
                    std::cout << "Client: Sent request (type 2) to "
                            << host << ":" << port << std::endl;
                }
                break;
            case 4:
                std::cout << "Client: Sent share request (type 4) to " << host << ":"
                    << port << std::endl;
                break;
            case 5:
                if(strcasecmp(request.requestString, "lookup") == 0) {
                    std::cout << "Client: Sent system lookup request (type 5) for content file \""
                            << request.payload << "\" to "
                            << host << ":" << port << std::endl;
                } else {
                    std::cout << "Client: Sent system ping request (type 5) to " << host << ":"
                        << port << std::endl;
                }
        }
    }

    void client_connection::printResponseMessage(service_request response) {
        if((int)strlen(response.requestString) > 0) {
            std::cout << "Client: Received response (type 3) with message \""
                    << response.requestString << "\" from " << host << ":" << port
                    << std::endl;
            // If found, print special found message (system-wide lookups only)
            if(strcasecmp(response.requestString, "found") == 0 && strlen(response.visited) > 0) {
                std::vector<std::string> visited = serializer.decodeNeighbors(response.visited);
                std::cout << "Client: File found on node " << visited.at(visited.size()-1)
                        << " (" << visited.size()-1 << " node(s) searched)" << std::endl;
            }
            // If found, print file contents (any lookup)
            if(strcasecmp(response.requestString, "found") == 0) {
                std::cout << "Client: File contents:\n\n"
                        << response.payload << "<end of file>\n" << std::endl;
            }
        } else {
            std::cout << "Client: Received response (type 3) from " << host << ":"
                    << port << std::endl;
        }
    }
}
