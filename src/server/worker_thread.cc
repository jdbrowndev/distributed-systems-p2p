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
#include "../graph_traversal.h"
#include "../globals.h"
#include "../service_request.h"
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
        writeResponse((char*)"", (char*)"");
    }

    void worker_thread::handleClientExitRequest() {
        std::cout << "Server: Received exit request from client " << client << std::endl;
        exit = true;
        writeResponse((char*)"", (char*)"");
    }

    void worker_thread::handleClientQueryRequest() {
        if(strcasecmp(request.requestString, "ping") == 0) {
            std::cout << "Server: Received ping request from client " << client << std::endl;
            writeResponse((char*)"alive", (char*)"");
        } else if(strcasecmp(request.requestString, "lookup") == 0) {
            std::cout << "Server: Received request for content file \"" << request.payload
                    << "\" from client " << client << std::endl;
            std::string fileContents = fileManager.readContentFile(std::string(request.payload));
            if(fileContents.length() > 0) {
                std::cout << "Server: Found content file \"" << request.payload
                        << "\" for client " << client << std::endl;
                writeResponse((char*)"found", (char*)fileContents.c_str());
            } else {
                std::cout << "Server: Could not find content file \"" << request.payload
                        << "\" for client " << client << std::endl;
                writeResponse((char*)"not found", (char*)"");
            }
        } else {
            std::cout << "Server: Received query request from " << client << std::endl;
            writeResponse((char*)"", (char*)"");
        }
    }

    void worker_thread::handleClientShareRequest() {
        if(strcasecmp(request.requestString, "neighbors") == 0) {
            std::cout << "Server: Received neighbor share request from client " << client << std::endl;
            appendSharedNeighbors();
            writeResponse((char*)"thanks", (char*)"");
        } else {
            std::cout << "Server: Received neighbor share request from client " << client
                    << ", but it is missing some information" << std::endl;
            writeResponse((char*)"error", (char*)"");
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
        std::string fileContents = isFileQuery ? fileManager.readContentFile(std::string(request.payload)) : "";
        if(fileContents.length() > 0) {
            std::cout << "Server: Found content file \"" << request.payload
                    << "\" for client " << client << std::endl;
            writeResponse((char*)"found", (char*)fileContents.c_str(), request.visited);
        } else {
            graph_traversal traversal(port);
            graph_traversal_result result = traversal.traverse(serializer.decodeNeighbors(request.visited),
                    std::string(request.payload));
            if(result.fileContents.length() > 0) {
                writeResponse((char*)"found", (char*)result.fileContents.c_str(),
                        (char*)serializer.encodeNeighbors(result.visited).c_str());
            } else {
                writeResponse(isFileQuery ? (char*)"not found" : (char*)"", (char*)"",
                        (char*)serializer.encodeNeighbors(result.visited).c_str());
            }
        }
    }

    void worker_thread::writeResponse(char* requestString, char* payload) {
        writeResponse(requestString, payload, (char*)"");
    }

    void worker_thread::writeResponse(char* requestString, char* payload, char* visited) {
        service_request response;
        gethostname(response.domainName, sizeof(response.domainName));
        response.portNumber = port;
        response.requestType = 3;
        response.requestId = 0;
        strncpy(response.requestString, requestString, sizeof(response.requestString));
        strncpy(response.payload, payload, sizeof(response.payload));
        strncpy(response.visited, visited, sizeof(response.visited));
        write(connection, (char*)&response, sizeof(service_request));
        if(strlen(requestString) > 0) {
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
        std::vector<std::string> sharedNeighbors = serializer.decodeNeighbors(request.payload);
        for(std::vector<std::string>::iterator it = sharedNeighbors.begin();
                it != sharedNeighbors.end(); it++) {
            std::string neighbor = *it;
            if(!tryAppendNeighbor(neighbor)) {
                std::cout << "Server: Cannot add " << neighbor << " to neighbors list: "
                        << neighbor << " is already in the list" << std::endl;
            }
        }
    }

    bool worker_thread::tryAppendNeighbor(std::string neighbor) {
        if(std::find(neighbors.begin(), neighbors.end(), neighbor) == neighbors.end()) {
            appendToNeighborsVector(neighbor);
            fileManager.appendNeighborToFile(neighbor);
            return true;
        }
        return false;
    }
}
