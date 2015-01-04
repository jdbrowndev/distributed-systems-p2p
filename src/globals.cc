//
// globals.cc
//
// Author: Jordan Brown
// Date: Sep 4, 2014
//
// Stores global variables and procedures

#include <vector>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <iostream>
#include <pthread.h>
#include "globals.h"
#include "file_manager.h"
#include "neighbors_vector.h"

using namespace brown;

neighbors_vector neighbors;
file_manager fileManager;

void initGlobals(std::vector<std::string> initialNeighbors) {
    neighbors = neighbors_vector(initialNeighbors);
}
