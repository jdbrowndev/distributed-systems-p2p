//
// server.cc
//
// Author: Jordan Brown
// Date: Sep 1, 2014
//
// Entry point for server application

#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "globals.h"
#include "server/request_handler.h"
#include "server/server_connection.h"
#include "client/client_connection.h"
#include "service_request.h"

using namespace brown;

void validateInput(char* lowPort, char* highPort);

int main(int argc, char** argv) {

	// TODO: these values should be pulled from 'portnums' file
	// TODO: load hosts from 'neighbors' file
	char lowPort[] = "2000";
	char highPort[] = "3000";
	validateInput(lowPort, highPort);
	initGlobals();

	server_connection serverConnection(atoi(lowPort), atoi(highPort));
	char* port = serverConnection.openConnection();

	// TODO: launch client stuff here

	request_handler handler(serverConnection.getSocketDesc(), port);
	handler.serviceRequests();
}

void validateInput(char* lowPort, char* highPort) {
	int lowPortInt = atoi(lowPort);
	int highPortInt = atoi(highPort);
	if(strlen(lowPort) == 0 || strlen(highPort) == 0) {
		std::cout << "The portnums file must provide lower and upper port boundaries"
				<< std::endl;
		exit(0);
	}
	if(lowPortInt > highPortInt) {
		std::cout << "The lower port boundary must be less than the higher port boundary"
				<< std::endl;
		exit(0);
	}
	if(lowPortInt < PORT_MINIMUM || highPortInt < PORT_MINIMUM) {
		std::cout << "Port number boundaries must be greater than or equal to "
				<< PORT_MINIMUM << std::endl;
		exit(0);
	} else if (lowPortInt > PORT_MAXIMUM || highPortInt > PORT_MAXIMUM) {
		std::cout << "Port number boundaries must be less than or equal to "
				<< PORT_MAXIMUM << std::endl;
		exit(0);
	}
}
