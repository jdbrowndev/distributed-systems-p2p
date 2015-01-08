//
// neighbors_vector.h
//
// Author: Jordan Brown
// Date: Dec 31, 2014
//
// A thread-safe vector for storing known neighbors

#include <pthread.h>
#include <vector>
#include <string>
#include "print_helper.h"

#ifndef NEIGHBORS_VECTOR_H_
#define NEIGHBORS_VECTOR_H_ 

namespace brown {
    class neighbors_vector {
        private:
            std::vector<std::string>* neighborsPtr;
            std::vector<std::string> neighbors;
            pthread_mutex_t neighborsMutex;
            print_helper printHelper;
            void init();
        public:
            neighbors_vector();
            neighbors_vector(std::vector<std::string>* vectorPtr);
            std::string at(int index);
            void append(std::string neighbor);
            bool contains(std::string neighbor);
            int size();
            void copy(std::vector<std::string>& out);
            void print();
    };
} /* namespace brown */

#endif
