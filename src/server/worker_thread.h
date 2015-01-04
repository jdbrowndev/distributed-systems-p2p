//
// worker_thread.h
//
// Author: Jordan Brown
// Date: Sep 6, 2014
//
// Handles reads/writes to other peers

#ifndef WORKER_THREAD_H_
#define WORKER_THREAD_H_

#include <string>
#include "../neighbor_serializer.h"
#include "../service_request.h"

namespace brown {
    class worker_thread {
    private:
        int connection;
        int port; // The port this node is currently listening on
        service_request request;
        std::string client;
        bool exit;
        neighbor_serializer serializer;
        void handleRequest();
        void handleNewClientRequest();
        void handleClientExitRequest();
        void handleClientQueryRequest();
        void handleClientShareRequest();
        void handleClientSystemQueryRequest();
        void writeResponse();
        void writeResponse(std::string requestString);
        void writeResponse(std::string requestString, std::string payload);
        void writeResponse(std::string requestString, std::string payload, std::string visited);
        void buildClientString();
        void appendSharedNeighbors();
        bool tryAppendNeighbor(std::string neighbor);
    public:
        worker_thread(int connection, int port);
        void handleRequests();
    };
}

#endif /* WORKER_THREAD_H_ */
