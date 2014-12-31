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

void printStringVector(std::vector<std::string> vector, std::string title, bool numbered) {
    const int titlePadding = 5;
    const int titleDecorLength = title.length() + titlePadding * 2;
    std::stringstream outputStr;
    outputStr << "\n";
    for(int i = 1; i <= titleDecorLength; i++) {
        outputStr << "=";
    }
    outputStr << "\n";
    for(int i = 1; i <= titlePadding; i++) {
        outputStr << " ";
    }
    outputStr << title << "\n";
    for(int i = 1; i <= titleDecorLength; i++) {
        outputStr << "=";
    }
    outputStr << "\n";
    int counter = 1;
    for(std::vector<std::string>::iterator it = vector.begin();
            it != vector.end(); it++) {
        if(numbered) {
            outputStr << counter << ". ";
            counter++;
        }
        outputStr << *it << "\n";
    }
    std::cout << outputStr.rdbuf();
}
