//
// network_connection.h
//
// Author: Jordan Brown
// Date: Sep 27, 2014
//
// Parent class for TCP/IP connections

#ifndef NETWORK_CONNECTION_H_
#define NETWORK_CONNECTION_H_

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

namespace brown {
    class network_connection {
        public:
            int socketdesc;
        protected:
            addrinfo* myinfo;
            network_connection();
            bool openSocket();
            bool setAddressInfo(std::string host, std::string port);
    };
}

#endif /* NETWORK_CONNECTION_H_ */
