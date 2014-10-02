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
}
extern pthread_mutex_t neighborsMutex;
extern std::vector<std::string> neighbors;
extern void initGlobals();

#endif /* GLOBALS_H_ */
