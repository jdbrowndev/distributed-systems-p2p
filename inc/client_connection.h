//
// client_connection.h
//
// Author: Jordan Brown
// Date: Sep 3, 2014
//
// Connects to a peer

#ifndef CLIENT_CONNECTION_H_
#define CLIENT_CONNECTION_H_

#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "network_connection.h"
#include "neighbor_serializer.h"
#include "service_request.h"

namespace brown {
    class client_connection : public network_connection {
        private:
            std::string host;
            std::string port;
            int connection;
            neighbor_serializer serializer;
        public:
            client_connection(std::string host, std::string port);
            bool openConnection();
            void closeConnection();
            service_request sendRequest(service_request request);
            void printRequestMessage(service_request request);
            void printResponseMessage(service_request response);
    };
}

#endif /* CLIENT_CONNECTION_H_ */
