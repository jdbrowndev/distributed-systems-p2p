//
// neighbor_serializer.h
//
// Author: Jordan Brown
// Date: Dec 30, 2014
//
// Encodes and decodes neighbors for transmission

#ifndef NEIGHBOR_SERIALIZER_H_
#define NEIGHBOR_SERIALIZER_H_ 

#include <stdio.h>
#include <string>
#include <vector>

namespace brown {
    class neighbor_serializer {
        public:
            struct host_port_tokens {
                std::string host;
                std::string port;
            };
            std::string encodeNeighbors(std::vector<std::string>& neighborsVector);
            std::string encodeNeighbors(std::vector<std::string>& neighborsVector, int max);
            void decodeNeighbors(std::string neighborsString, std::vector<std::string>& out);
            host_port_tokens splitNeighbor(std::string neighbor);
    };
} /* namespace brown */

#endif
