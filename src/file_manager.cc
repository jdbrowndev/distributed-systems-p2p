//
// file_manager.cc
//
// Author: Jesse Rowland
// Date: Oct 1, 2014
//
// Handles file write/reads

#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include "globals.h"
#include "file_manager.h"

namespace brown {
	void file_manager::openPortNumsFile(){
		portnumsFile.open("portnums.txt");
	}
	
	void file_manager::closePortNumsFile(){
		portnumsFile.close();
		return;
	}
	
	void file_manager::openNeighborsFile(){
		neighborsFile.open("neighborsFile.txt");
	}
	
	void file_manager::closeNeighborsFile(){
		neighborsFile.close();
		return;
	}
	
	int* file_manager::readPortNumsFile(int ports[]){
		string line;
		for(int i = 0; i < 2; i++)
		{
			getline(portnumsFile, line);
			istringstream(line) >> ports[i];
		}
		return ports;
	}
	
	void file_manager::readNeighborsFile(){
		string line;
		while(getline(neighborsFile, line)){
			neighbors.push_back(line);
		}
		return;
	}
	
	void file_manager::appendNeighborToFile(string neighbor){
		neighborsFile << neighbor;
		neighborsFile << "\n";
		return;
	}
}
