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
pthread_mutex_t neighborsFileMutex;
std::vector<std::string> neighbors;
file_manager fileManager;

void initGlobals() {
	pthread_mutex_init(&neighborsMutex, NULL);
	pthread_mutex_init(&neighborsFileMutex, NULL);
}

void appendToNeighborsVector(std::string neighbor) {
	pthread_mutex_lock(&neighborsMutex);
		neighbors.push_back(neighbor);
	pthread_mutex_unlock(&neighborsMutex);
}

void appendToNeighborsFile(std::string neighbor) {
	pthread_mutex_lock(&neighborsFileMutex);
		fileManager.openNeighborsFile();
		fileManager.appendNeighborToFile(neighbor);
		fileManager.closeNeighborsFile();
	pthread_mutex_unlock(&neighborsFileMutex);
}

void printNeighbors() {
	std::cout << "Neighbors:" << std::endl;
	const int indent = 3;
	int count = 1;
	for(std::vector<std::string>::iterator it = neighbors.begin();
			it != neighbors.end(); it++) {
		for(int i = 1; i <= indent; i++) {
			std::cout << " ";
		}
		std::cout << count << ". " << *it << std::endl;
		count++;
	}
}
