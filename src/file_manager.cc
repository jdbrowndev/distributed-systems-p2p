//
// file_manager.cc
//
// Author: Jesse Rowland
// Date: Oct 1, 2014
//
// Handles file write/reads

#include <string>
#include <sstream>
#include <fstream>
#include "globals.h"
#include "file_manager.h"

namespace brown {
    const char* file_manager::PORTNUMS_FILE_NAME = "config/portnums.txt";
    const char* file_manager::NEIGHBORS_FILE_NAME = "config/neighbors.txt";
    const char* file_manager::CONTENT_FILE_DIRECTORY = "content";

    file_manager::file_manager() { }

    void file_manager::openPortNumsFile(){
        portnumsFile.open(PORTNUMS_FILE_NAME);
    }
    
    void file_manager::closePortNumsFile(){
        portnumsFile.close();
        return;
    }
    
    void file_manager::openNeighborsFile(){
        neighborsFile.open(NEIGHBORS_FILE_NAME, std::fstream::in |
                std::fstream::app | std::fstream::out);
    }
    
    void file_manager::closeNeighborsFile(){
        neighborsFile.close();
        return;
    }
    
    std::string* file_manager::readPortNumsFile(std::string ports[]){
        std::string line;
        for(int i = 0; i < 2; i++)
        {
            getline(portnumsFile, line);
            ports[i] = line;
        }
        return ports;
    }
    
    void file_manager::readNeighborsFile(){
        std::string line;
        while(getline(neighborsFile, line)){
            neighbors.push_back(line);
        }
        return;
    }
    
    void file_manager::appendNeighborToFile(std::string neighbor){
        neighborsFile << neighbor;
        neighborsFile << "\n";
        return;
    }

    std::string file_manager::readContentFile(std::string fileName) {
        std::string path = std::string(CONTENT_FILE_DIRECTORY) + "/" + fileName;
        contentFile.open(path.c_str());
        if(!contentFile) {
            return "";
        } else {
            std::stringstream buffer;
            buffer << contentFile.rdbuf();
            contentFile.close();
            return buffer.str();
        }
    }
}
