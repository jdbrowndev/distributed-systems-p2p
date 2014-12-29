//
// request_handler.h
//
// Author: Jordan Brown
// Date: Sep 4, 2014
//
// Accepts and handles connections from other peers

#ifndef REQUEST_HANDLER_H_
#define REQUEST_HANDLER_H_

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "../service_request.h"

namespace brown {
    class request_handler {
    private:
        struct worker_thread_args {
            int connection;
            int port;
        };
        char* port;
        int socketdesc;
        int connection;
        bool stepToPort();
        static void* launchWorkerThread(void*);
    public:
        request_handler(int socketdesc, char* port);
        void serviceRequests();
    };
}

#endif /* REQUEST_HANDLER_H_ */
