//
// main.cc
//
// Author: Jordan Brown
// Date: Sep 1, 2014
//
// Entry point for peer

#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <string>
#include "globals.h"
#include "server/request_handler.h"
#include "server/server_connection.h"
#include "client/client_interface.h"
#include "service_request.h"

using namespace brown;

void validateInput(char* lowPort, char* highPort);
bool hasClientArg(int argc, char** argv);
void* launchClientInterface(void* args);

int main(int argc, char** argv) {
    std::string portnums[2];
    fileManager.readPortNums(portnums); 
    char* lowPort = &portnums[0][0];
    char* highPort = &portnums[1][0];
    validateInput(lowPort, highPort);

    initGlobals(fileManager.readNeighbors());

    server_connection serverConnection(atoi(lowPort), atoi(highPort));
    serverConnection.openConnection();

    if(hasClientArg(argc, argv)) {
        pthread_t clientInterfaceThread;
        pthread_create(&clientInterfaceThread, NULL, launchClientInterface, 
                (void*)&serverConnection.port);
    }
    else {
        std::cout << "No command line parameter given, using server-only mode\n"
            << "The server will continue running until manually terminated\n" << std::endl;
    }
    
    request_handler handler(serverConnection.socketdesc, serverConnection.port);
    handler.serviceRequests();
}

void validateInput(char* lowPort, char* highPort) {
    int lowPortInt = atoi(lowPort);
    int highPortInt = atoi(highPort);
    if(strlen(lowPort) == 0 || strlen(highPort) == 0) {
        std::cout << "portnums: The file must provide lower and upper port boundaries"
                << std::endl;
        exit(0);
    }
    if(lowPortInt > highPortInt) {
        std::cout << "portnums: The lower port boundary must be less than the higher port boundary"
                << std::endl;
        exit(0);
    }
    if(lowPortInt < PORT_MINIMUM || highPortInt < PORT_MINIMUM) {
        std::cout << "portnums: Port number boundaries must be greater than or equal to "
                << PORT_MINIMUM << std::endl;
        exit(0);
    } else if (lowPortInt > PORT_MAXIMUM || highPortInt > PORT_MAXIMUM) {
        std::cout << "portnums: Port number boundaries must be less than or equal to "
                << PORT_MAXIMUM << std::endl;
        exit(0);
    }
}

bool hasClientArg(int argc, char** argv) {
    opterr = 0;
    char flag;
    while((flag = getopt(argc, argv, "c")) != -1) {
        switch(flag) {
            case 'c':
                return true;
        }
    }
    return false;
}

void* launchClientInterface(void* args) {
    std::string port = *((std::string*)args);
    client_interface interface(port);
    interface.initialize();
    pthread_exit(0);
    return NULL;
}
