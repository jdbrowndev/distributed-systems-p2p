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
#include "neighbors_vector.h"

namespace brown {
    static const int HOST_MAX_LENGTH = 255;
    static const int PORT_MAX_LENGTH = 5;
    static const int PORT_MINIMUM = 1024;
    static const int PORT_MAXIMUM = 65535;
    static const int PATH_MAX = 4096;
}
extern brown::neighbors_vector neighbors;
extern brown::file_manager fileManager;
extern void initGlobals(std::vector<std::string>& initialNeighbors);

#endif /* GLOBALS_H_ */
