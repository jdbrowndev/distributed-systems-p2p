//
// globals.cc
//
// Author: Jordan Brown
// Date: Sep 4, 2014
//
// Stores global variables

#include <vector>
#include <string>
#include <iostream>
#include <pthread.h>
#include "globals.h"

using namespace brown;

pthread_mutex_t connectedClientsMutex;
std::vector<std::string> connectedClients;

void initGlobals() {
	pthread_mutex_init(&connectedClientsMutex, NULL);
}
