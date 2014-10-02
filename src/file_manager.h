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

namespace brown {
	class file_manager {
	private:
		static const char* PORTNUMS_FILE_NAME;
		static const char* NEIGHBORS_FILE_NAME;
	public:
		file_manager();
		void openPortNumsFile();
		void closePortNumsFile();
		void openNeighborsFile();
		void closeNeighborsFile();
		std::string* readPortNumsFile(std::string ports[]);
		void readNeighborsFile();
		void appendNeighborToFile(std::string neighbor);
	protected:
		std::fstream portnumsFile;
		std::fstream neighborsFile;
	};
}

#endif /* FILE_MANAGER_H_ */
