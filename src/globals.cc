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
#include "file_manager.h"

using namespace brown;

pthread_mutex_t neighborsMutex;
pthread_mutex_t filesMutex;
std::vector<std::string> neighbors;
file_manager filemgr;

void initGlobals() {
	pthread_mutex_init(&neighborsMutex, NULL);
	pthread_mutex_init(&filesMutex, NULL);
}
