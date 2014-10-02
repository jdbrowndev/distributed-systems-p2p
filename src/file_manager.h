//
// file_manager.h
//
// Author: Jesse Rowland
// Date: Oct 1, 2014
//
// Handles file write/reads

#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

namespace brown {
	class file_manager {
	public:
		void openPortNumsFile();
		void closePortNumsFile();
		void openNeighborsFile();
		void closeNeighborsFile();
		int* readPortNumsFile(int ports[]);
		void readNeighborsFile();
		void appendNeighborToFile(string neighbor);
	protected:
		ifstream portnumsFile;
		fstream neighborsFile;
	};
}

#endif /* FILE_MANAGER_H_ */
