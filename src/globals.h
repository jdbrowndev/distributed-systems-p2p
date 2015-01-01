//
// globals.h
//
// Author: Jordan Brown
// Date: Sep 4, 2014
//
// Stores global variables and procedures

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <string>
#include <vector>
#include "file_manager.h"

namespace brown {
    static const int HOST_MAX_LENGTH = 255;
    static const int PORT_MAX_LENGTH = 5;
    static const int PORT_MINIMUM = 1024;
    static const int PORT_MAXIMUM = 65535;
}
extern pthread_mutex_t neighborsMutex;
extern std::vector<std::string> neighbors;
extern brown::file_manager fileManager;
extern void initGlobals();
extern void appendToNeighborsVector(std::string neighbor);
extern void printStringVector(std::vector<std::string> vector, std::string title, bool numbered);

#endif /* GLOBALS_H_ */
