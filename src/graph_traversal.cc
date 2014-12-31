//
// graph_traversal.cc
//
// Author: Jordan Brown
// Date: Nov 23, 2014
//
// Traverses the entire network of peers

#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <pthread.h>
#include "client/client_connection.h"
#include "globals.h"
#include "neighbor_serializer.h"
#include "service_request.h"
#include "graph_traversal.h"

namespace brown {
    graph_traversal::graph_traversal(int port):port(port) { }

    graph_traversal_result graph_traversal::traverse(std::vector<std::string> visited, std::string 
            fileName) {
        std::string fileContents = "";
        // Deep copying the neighbors vector is much safer than locking it
        // for the entire duration of the for loop.
        std::vector<std::string> neighborsCopy = neighbors;
        for(std::vector<std::string>::iterator it = neighborsCopy.begin(); it != neighborsCopy.end(); it++) {
            std::string neighbor = *it;
            // Query neighbor only if it is not in the visited list
            if(std::find(visited.begin(), visited.end(), neighbor) == visited.end()) {
                neighbor_serializer::host_port_tokens neighborTokens = serializer.splitNeighbor(neighbor);
                // Note: this connection is separate from the connection maintained by client_interface.cc
                client_connection connection((char*)neighborTokens.host.c_str(),
                        (char*)neighborTokens.port.c_str());
                if(connection.openConnection()) {
                    visited.push_back(neighbor);
                    service_request response = sendTraverseRequest(connection, visited, fileName);
                    connection.closeConnection();
                    visited = serializer.decodeNeighbors(std::string(response.visited));
                    if(strcasecmp(response.requestString, "found") == 0) {
                        fileContents = std::string(response.payload);
                        break;
                    }
                }
            }
        }
        return graph_traversal_result(visited, fileContents);
    }

    // Precondition: connection must be valid and open
    service_request graph_traversal::sendTraverseRequest(client_connection &connection, 
            std::vector<std::string> &visited, std::string fileName) {
        connection.sendRequest(createServiceRequest(0, (char*)"", (char*)"", (char*)""));
        service_request response = connection.sendRequest(createServiceRequest(5,
                fileName.length() > 0 ? (char*)"lookup" : (char*)"",
                (char*)fileName.c_str(),
                (char*)serializer.encodeNeighbors(visited).c_str()));
        connection.sendRequest(createServiceRequest(1, (char*)"", (char*)"", (char*)""));
        return response;
    }

    service_request graph_traversal::createServiceRequest(int requestType, char* requestString, 
            char* payload, char* visitedString) {
        service_request request;
        gethostname(request.domainName, sizeof(request.domainName));
        request.portNumber = port;
        request.requestId = 0;
        request.requestType = requestType;
        strncpy(request.requestString, requestString, sizeof(request.requestString));
        strncpy(request.payload, payload, sizeof(request.payload));
        strncpy(request.visited, visitedString, sizeof(request.visited));
        return request;
    }
}
