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
        std::vector<std::string> neighborsCopy; 
        neighbors.copy(neighborsCopy);
        for(std::vector<std::string>::iterator it = neighborsCopy.begin(); it != neighborsCopy.end(); it++) {
            std::string neighbor = *it;
            // Query neighbor only if it is not in the visited list
            if(std::find(visited.begin(), visited.end(), neighbor) == visited.end()) {
                neighbor_serializer::host_port_tokens neighborTokens = serializer.splitNeighbor(neighbor);
                // Note: this connection is separate from the connection maintained by client_interface.cc
                client_connection connection(neighborTokens.host, neighborTokens.port);
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
        connection.sendRequest(createServiceRequest(port, 0));
        service_request response = connection.sendRequest(createServiceRequest(port, 5,
                fileName.length() > 0 ? "lookup" : "", fileName, serializer.encodeNeighbors(visited)));
        connection.sendRequest(createServiceRequest(port, 1));
        return response;
    }
}
