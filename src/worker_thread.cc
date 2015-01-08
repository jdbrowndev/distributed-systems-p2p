//
// worker_thread.cc
//
// Author: Jordan Brown
// Date: Sep 6, 2014
//
// Handles reads/writes to other peers

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include "graph_traversal.h"
#include "globals.h"
#include "service_request.h"
#include "worker_thread.h"

namespace brown {
    worker_thread::worker_thread(int connection, int port):connection(connection), port(port), exit(false) { }

    void worker_thread::handleRequests() {
        while(!exit) {
            read(connection, (char*)&request, sizeof(service_request));
            if(client.length() == 0) { buildClientString(); }
            handleRequest();
        }
        close(connection);
    }

    void worker_thread::handleRequest() {
        switch(request.requestType) {
            case 0:
                handleNewClientRequest();
                break;
            case 1:
                handleClientExitRequest();
                break;
            case 2:
                handleClientQueryRequest();
                break;
            case 4:
                handleClientShareRequest();
                break;
            case 5:
                handleClientSystemQueryRequest();
                break;
        }
    }

    void worker_thread::handleNewClientRequest() {
        std::cout << "Server: Received request from new client " << client << std::endl;
        tryAppendNeighbor(client);
        writeResponse();
    }

    void worker_thread::handleClientExitRequest() {
        std::cout << "Server: Received exit request from client " << client << std::endl;
        exit = true;
        writeResponse();
    }

    void worker_thread::handleClientQueryRequest() {
        if(strcasecmp(request.requestString, "ping") == 0) {
            std::cout << "Server: Received ping request from client " << client << std::endl;
            writeResponse("alive");
        } else if(strcasecmp(request.requestString, "lookup") == 0) {
            std::cout << "Server: Received request for content file \"" << request.payload
                << "\" from client " << client << std::endl;
            std::string fileContents = fileManager.readContent(std::string(request.payload));
            if(fileContents.length() > 0) {
                std::cout << "Server: Found content file \"" << request.payload
                    << "\" for client " << client << std::endl;
                writeResponse("found", fileContents);
            } else {
                std::cout << "Server: Could not find content file \"" << request.payload
                    << "\" for client " << client << std::endl;
                writeResponse("not found");
            }
        } else {
            std::cout << "Server: Received query request from " << client << std::endl;
            writeResponse();
        }
    }

    void worker_thread::handleClientShareRequest() {
        if(strcasecmp(request.requestString, "neighbors") == 0) {
            std::cout << "Server: Received neighbor share request from client " << client << std::endl;
            appendSharedNeighbors();
            writeResponse("thanks");
        } else {
            std::cout << "Server: Received neighbor share request from client " << client
                << ", but it is missing some information" << std::endl;
            writeResponse("error");
        }
    }

    void worker_thread::handleClientSystemQueryRequest() {
        bool isFileQuery = strcasecmp(request.requestString, "lookup") == 0;
        if(isFileQuery) {
            std::cout << "Server: Received system lookup request for content file \"" << request.payload
                << "\" from client " << client << std::endl;
        } else {
            std::cout << "Server: Received system ping request from client " << client << std::endl;
        }
        std::string fileContents = isFileQuery ? fileManager.readContent(std::string(request.payload)) : "";
        if(fileContents.length() > 0) {
            std::cout << "Server: Found content file \"" << request.payload
                << "\" for client " << client << std::endl;
            writeResponse("found", fileContents, request.visited);
        } else {
            graph_traversal traversal(port);
            std::vector<std::string> decodedVisited;
            serializer.decodeNeighbors(request.visited, decodedVisited);
            graph_traversal_result result = traversal.traverse(decodedVisited,
                    std::string(request.payload));
            if(result.fileContents.length() > 0) {
                writeResponse("found", result.fileContents, 
                        serializer.encodeNeighbors(result.visited));
            } else {
                writeResponse(isFileQuery ? "not found" : "", "",
                        serializer.encodeNeighbors(result.visited));
            }
        }
    }

    void worker_thread::writeResponse() {
        writeResponse("", "", "");
    }

    void worker_thread::writeResponse(std::string requestString) {
        writeResponse(requestString, "", "");
    }

    void worker_thread::writeResponse(std::string requestString, std::string payload) {
        writeResponse(requestString, payload, "");
    }

    void worker_thread::writeResponse(std::string requestString, std::string payload, std::string visited) {
        service_request response = createServiceRequest(port, 3, requestString, payload, visited);
        write(connection, (char*)&response, sizeof(service_request));
        if(requestString.length() > 0) {
            std::cout << "Server: Response with message \"" << requestString << "\" sent to "
                << client << std::endl;
        } else {
            std::cout << "Server: Response sent to " << client << std::endl;
        }
    }


    void worker_thread::buildClientString() {
        std::stringstream strStream;
        strStream << request.domainName << ":" << request.portNumber;
        client = strStream.str();
    }

    void worker_thread::appendSharedNeighbors() {
        std::vector<std::string> sharedNeighbors;
        serializer.decodeNeighbors(request.payload, sharedNeighbors);
        for(std::vector<std::string>::iterator it = sharedNeighbors.begin();
                it != sharedNeighbors.end(); ++it) {
            std::string neighbor = *it;
            if(!tryAppendNeighbor(neighbor)) {
                std::cout << "Server: Cannot add " << neighbor << " to neighbors list: "
                    << neighbor << " is already in the list" << std::endl;
            }
        }
    }

    bool worker_thread::tryAppendNeighbor(std::string neighbor) {
        if(neighbors.contains(neighbor)) {
            neighbors.append(neighbor);
            fileManager.appendNeighbor(neighbor);
            return true;
        }
        return false;
    }
}
