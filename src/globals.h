//
// globals.h
//
// Author: Jordan Brown
// Date: Sep 4, 2014
//
// Stores global variables

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <string>
#include <vector>

namespace brown {
	static const int PORT_MAX_LENGTH = 5;
	static const int PORT_MINIMUM = 1024;
	static const int PORT_MAXIMUM = 65535;
	static const char* DOMAIN_NAME = "einstein.etsu.edu";
}
extern pthread_mutex_t connectedClientsMutex;
extern std::vector<std::string> connectedClients;
extern void initGlobals();

#endif /* GLOBALS_H_ */
