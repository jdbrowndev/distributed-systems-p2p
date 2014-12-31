//
// neighbor_serializer.cc
//
// Author: Jordan Brown
// Date: Dec 30, 2014
//
// Encodes and decodes neighbors for transmission

#include <string>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <iostream>
#include "neighbor_serializer.h"

namespace brown {
    std::string neighbor_serializer::encodeNeighbors(std::vector<std::string> neighborsVector) {
        return encodeNeighbors(neighborsVector, -1);
    }

    std::string neighbor_serializer::encodeNeighbors(std::vector<std::string> neighborsVector, int max) {
        if (neighborsVector.size() == 0) {
            return "0";
        } else {
            std::stringstream outputStream;
            int numNeighbors = (neighborsVector.size() <= max || max < 0) ? neighborsVector.size() : max;
            outputStream << numNeighbors;
            for(int i = 1; i <= numNeighbors; i++) {
                host_port_tokens neighborTokens = splitNeighbor(neighborsVector.at(i-1));
                outputStream << ";" << neighborTokens.host << ";" << neighborTokens.port;
            }
            return outputStream.str(); 
        }
    }

    std::vector<std::string> neighbor_serializer::decodeNeighbors(std::string neighborsString) {
        std::vector<std::string> output;
        int neighborsCount = atoi(strtok((char*)neighborsString.c_str(), ";"));
        std::stringstream strStream;
        for(int i = 1; i <= neighborsCount; i++) {
            char* host = strtok(NULL, ";");
            char* port = strtok(NULL, ";");
            strStream << host << ":" << port;
            output.push_back(strStream.str());
            strStream.str("");
        }
        return output;
    }

    neighbor_serializer::host_port_tokens neighbor_serializer::splitNeighbor(std::string neighbor) {
        host_port_tokens output;
        std::stringstream stringStr(neighbor);
        getline(stringStr, output.host, ':');
        getline(stringStr, output.port, ':');
        return output;
    }
} /* namespace brown */
