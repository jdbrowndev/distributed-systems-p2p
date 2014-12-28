//
// network_connection.h
//
// Author: Jordan Brown
// Date: Sep 27, 2014
//
// Parent class for TCP/IP connections

#ifndef NETWORK_CONNECTION_H_
#define NETWORK_CONNECTION_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

namespace brown {
    class network_connection {
    public:
        int getSocketDesc();
    protected:
        int socketdesc;
        addrinfo* myinfo;
        network_connection();
        void openSocket();
        bool setAddressInfo(char* host, char* port);
    };
}

#endif /* NETWORK_CONNECTION_H_ */
