//
// graph_traversal.h
//
// Author: Jordan Brown
// Date: Nov 23, 2014
//
// Traverses the entire network of peers

#ifndef GRAPH_TRAVERSAL_H_
#define GRAPH_TRAVERSAL_H_

#include <string>
#include <vector>
#include "client/client_connection.h"
#include "service_request.h"
#include "globals.h"

namespace brown {
    struct graph_traversal_result {
        graph_traversal_result();
        graph_traversal_result(std::vector<std::string> visited, std::string fileContents):
            visited(visited), fileContents(fileContents) { }
        std::vector<std::string> visited;
        std::string fileContents;
    };
    class graph_traversal {
    private:
        int port; // The port this node is currently listening on
        service_request createServiceRequest(int requestType, char* requestString, char* payload, char* visitedString);
        service_request sendTraverseRequest(client_connection &connection, std::vector<std::string> &visited,
                std::string fileName);
    public:
        graph_traversal(int port);
        graph_traversal_result traverse(std::vector<std::string> visited, std::string fileName);
    };
}

#endif /* GRAPH_TRAVERSAL_H_ */
