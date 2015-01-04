//
// service_request.cc
//
// Author: Jordan Brown
// Date: Sep 3, 2014
//
// Data structure used for communication between peers

#include <string.h>
#include <string>
#include "service_request.h"

namespace brown {
    service_request createServiceRequest(int port, int requestType) {
        return createServiceRequest(port, requestType, "", "", ""); 
    }

    service_request createServiceRequest(int port, int requestType, std::string requestString) {
        return createServiceRequest(port, requestType, requestString, "", ""); 
    }

    service_request createServiceRequest(int port, int requestType, std::string requestString, 
            std::string payload) {
        return createServiceRequest(port, requestType, requestString, payload, "");
    } 

    service_request createServiceRequest(int port, int requestType, std::string requestString, 
            std::string payload, std::string visited) {

        service_request output;
        output.requestType = requestType;
        gethostname(output.domainName, sizeof(output.domainName));
        output.portNumber = port;

        // Fill strings with null character 
        memset(&output.requestString[0], 0, sizeof(output.requestString));
        memset(&output.payload[0], 0, sizeof(output.payload));
        memset(&output.visited[0], 0, sizeof(output.visited));

        // Copy strings
        requestString.copy(output.requestString, sizeof(output.requestString) - 1, 0);
        payload.copy(output.payload, sizeof(output.payload) - 1, 0);
        visited.copy(output.visited, sizeof(output.visited) - 1, 0);

        return output;
    }
}
