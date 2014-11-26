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

std::string encodeNeighbors(std::vector<std::string> neighborsVector) {
	return encodeNeighbors(neighborsVector, -1);
}

std::string encodeNeighbors(std::vector<std::string> neighborsVector, int max) {
	if (neighborsVector.size() == 0) {
		return "0";
	} else {
		std::stringstream outputStream;
		int numNeighbors = (neighborsVector.size() <= max || max < 0) ? neighborsVector.size() : max;
		outputStream << numNeighbors;
		for(int i = 1; i <= numNeighbors; i++) {
			host_port_tokens neighborTokens = splitNeighbor(neighborsVector.at(i-1));
			outputStream << ";" << neighborTokens.host << ";" << neighborTokens.port;
		}
		return (char*)outputStream.str().c_str();
	}
}

std::vector<std::string> decodeNeighbors(std::string neighborsString) {
	std::vector<std::string> outputVector;
	int neighborsCount = atoi(strtok((char*)neighborsString.c_str(), ";"));
	std::stringstream strStream;
	for(int i = 1; i <= neighborsCount; i++) {
		char* host = strtok(NULL, ";");
		char* port = strtok(NULL, ";");
		strStream << host << ":" << port;
		outputVector.push_back(strStream.str());
		strStream.str("");
	}
	return outputVector;
}

host_port_tokens splitNeighbor(std::string neighbor) {
	host_port_tokens output;
	std::stringstream stringStr(neighbor);
	getline(stringStr, output.host, ':');
	getline(stringStr, output.port, ':');
	return output;
}

void printVectorStrings(std::vector<std::string> vector, std::string title, bool numbered) {
	const int padding = 5;
	std::stringstream outputStr;
	outputStr << "\n";
	for(int i = 1; i <= title.length() + padding*2; i++) {
		outputStr << "=";
	}
	outputStr << "\n";
	for(int i = 1; i <= padding; i++) {
		outputStr << " ";
	}
	outputStr << title << "\n";
	for(int i = 1; i <= title.length() + padding*2; i++) {
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
