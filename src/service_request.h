//
// service_request.h
//
// Author: Jordan Brown
// Date: Sep 3, 2014
//
// Data structure used for communication between peers

#ifndef SERVICE_REQUEST_H_
#define SERVICE_REQUEST_H_

#include <string>

namespace brown {
    struct service_request {
        char domainName[256];
        int portNumber;
        int requestType;
        char requestString[32];
        char payload[256];
        char visited[256]; // This will hold ~10 neighbors
    };

    extern service_request createServiceRequest(int port, int requestType);
    extern service_request createServiceRequest(int port, int requestType, std::string requestString);
    extern service_request createServiceRequest(int port, int requestType, std::string requestString, 
            std::string payload); 
    extern service_request createServiceRequest(int port, int requestType, std::string requestString,
            std::string payload, std::string visited);
}

#endif /* SERVICE_REQUEST_H_ */
