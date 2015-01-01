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
    const std::string file_manager::PORTNUMS_FILE_NAME = "config/portnums.txt";
    const std::string file_manager::NEIGHBORS_FILE_NAME = "config/neighbors.txt";
    const std::string file_manager::CONTENT_FILE_DIRECTORY = "content/";

    file_manager::file_manager() {
        pthread_mutex_init(&neighborsFileMutex, NULL);

        // Find program directory
        char buffer[PATH_MAX] = "";
        readlink("/proc/self/exe", buffer, PATH_MAX - 1);
        std::string tmp(buffer);
        // Eliminate "bin/main" from the string
        programDirectory = tmp.substr(0, tmp.size() - 8);
    }

    void file_manager::openPortNumsFile() {
        std::string path = programDirectory + PORTNUMS_FILE_NAME;
        portnumsFile.open(path.c_str());
    }
    
    void file_manager::closePortNumsFile() {
        portnumsFile.close();
    }
    
    void file_manager::openNeighborsFile() {
        std::string path = programDirectory + NEIGHBORS_FILE_NAME;
        neighborsFile.open(path.c_str(), std::fstream::in |
                std::fstream::app | std::fstream::out);
    }
    
    void file_manager::closeNeighborsFile() {
        neighborsFile.close();
    }

    void file_manager::openContentFile(std::string fileName) {
        std::string path = programDirectory + CONTENT_FILE_DIRECTORY + fileName;
        contentFile.open(path.c_str());
    }

    void file_manager::closeContentFile() {
        contentFile.close();
    }
    
    void file_manager::readPortNums(std::string ports[]) {
        std::string line;
        openPortNumsFile();
        for(int i = 0; i < 2; i++)
        {
            getline(portnumsFile, ports[i]);
        }
        closePortNumsFile();
    }
    
    std::vector<std::string> file_manager::readNeighbors() {
        std::vector<std::string> output;
        std::string line;
        pthread_mutex_lock(&neighborsFileMutex);
            openNeighborsFile();
            while(getline(neighborsFile, line)) {
                output.push_back(line);
            }
            closeNeighborsFile();
        pthread_mutex_unlock(&neighborsFileMutex);
        return output;
    }
    
    void file_manager::appendNeighbor(std::string neighbor) {
        pthread_mutex_lock(&neighborsFileMutex);
            openNeighborsFile();
            neighborsFile << neighbor;
            neighborsFile << "\n";
            closeNeighborsFile();
        pthread_mutex_unlock(&neighborsFileMutex);
    }

    std::string file_manager::readContent(std::string fileName) {
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
