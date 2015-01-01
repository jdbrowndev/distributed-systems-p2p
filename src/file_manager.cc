//
// file_manager.cc
//
// Author: Jordan Brown / Jesse Rowland
// Date: Dec 31, 2014
//
// Thread-safe class for handling file write/reads

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <pthread.h>
#include "globals.h"
#include "file_manager.h"

namespace brown {
    const char* file_manager::PORTNUMS_FILE_NAME = "config/portnums.txt";
    const char* file_manager::NEIGHBORS_FILE_NAME = "config/neighbors.txt";
    const char* file_manager::CONTENT_FILE_DIRECTORY = "content";

    file_manager::file_manager() {
        pthread_mutex_init(&neighborsFileMutex, NULL);
    }

    void file_manager::openPortNumsFile() {
        portnumsFile.open(PORTNUMS_FILE_NAME);
    }
    
    void file_manager::closePortNumsFile() {
        portnumsFile.close();
    }
    
    void file_manager::openNeighborsFile() {
        neighborsFile.open(NEIGHBORS_FILE_NAME, std::fstream::in |
                std::fstream::app | std::fstream::out);
    }
    
    void file_manager::closeNeighborsFile() {
        neighborsFile.close();
    }

    void file_manager::openContentFile(std::string fileName) {
        std::string path = std::string(CONTENT_FILE_DIRECTORY) + "/" + fileName;
        contentFile.open(path.c_str());
    }

    void file_manager::closeContentFile() {
        contentFile.close();
    }
    
    void file_manager::readPortNumsFile(std::string ports[]) {
        std::string line;
        openPortNumsFile();
        for(int i = 0; i < 2; i++)
        {
            getline(portnumsFile, ports[i]);
        }
        closePortNumsFile();
    }
    
    void file_manager::readNeighborsFile(std::vector<std::string> &vector) {
        std::string line;
        pthread_mutex_lock(&neighborsFileMutex);
            openNeighborsFile();
            while(getline(neighborsFile, line)) {
                vector.push_back(line);
            }
            closeNeighborsFile();
        pthread_mutex_unlock(&neighborsFileMutex);
    }
    
    void file_manager::appendNeighborToFile(std::string neighbor) {
        pthread_mutex_lock(&neighborsFileMutex);
            openNeighborsFile();
            neighborsFile << neighbor;
            neighborsFile << "\n";
            closeNeighborsFile();
        pthread_mutex_unlock(&neighborsFileMutex);
    }

    std::string file_manager::readContentFile(std::string fileName) {
        openContentFile(fileName);
        if(!contentFile) {
            return "";
        } else {
            std::stringstream buffer;
            buffer << contentFile.rdbuf();
            closeContentFile();
            return buffer.str();
        }
    }
}
