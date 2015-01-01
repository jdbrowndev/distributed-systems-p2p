//
// file_manager.h
//
// Author: Jordan Brown / Jesse Rowland
// Date: Dec 31, 2014
//
// Thread-safe class for handling file write/reads

#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>

namespace brown {
    class file_manager {
    private:
        static const std::string PORTNUMS_FILE_NAME;
        static const std::string NEIGHBORS_FILE_NAME;
        static const std::string CONTENT_FILE_DIRECTORY;
        std::string programDirectory; // The root directory of the program where bin, config, etc. lie
        std::fstream portnumsFile;
        std::fstream neighborsFile;
        std::fstream contentFile;
        // Only the neighbors file needs a mutex lock; all other files are read-only
        pthread_mutex_t neighborsFileMutex;
        void openPortNumsFile();
        void closePortNumsFile();
        void openNeighborsFile();
        void closeNeighborsFile();
        void openContentFile(std::string fileName);
        void closeContentFile();
    public:
        file_manager();
        void readPortNums(std::string ports[]);
        std::vector<std::string> readNeighbors();
        void appendNeighbor(std::string neighbor);
        std::string readContent(std::string fileName);
    };
}

#endif /* FILE_MANAGER_H_ */
