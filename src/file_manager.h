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
        static const char* PORTNUMS_FILE_NAME;
        static const char* NEIGHBORS_FILE_NAME;
        static const char* CONTENT_FILE_DIRECTORY;
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
        void readPortNumsFile(std::string ports[]);
        void readNeighborsFile(std::vector<std::string> &vector);
        void appendNeighborToFile(std::string neighbor);
        std::string readContentFile(std::string fileName);
    };
}

#endif /* FILE_MANAGER_H_ */
