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
            int neighborsCount = (neighborsVector.size() <= max || max < 0) ? neighborsVector.size() : max;
            outputStream << neighborsCount;
            for(int i = 1; i <= neighborsCount; ++i) {
                host_port_tokens neighborTokens = splitNeighbor(neighborsVector.at(i - 1));
                outputStream << ";" << neighborTokens.host << ";" << neighborTokens.port;
            }
            return outputStream.str(); 
        }
    }

    std::vector<std::string> neighbor_serializer::decodeNeighbors(std::string neighborsString) {
        std::stringstream tokenizer(neighborsString);
        std::string neighborsCountStr, host, port;
        getline(tokenizer, neighborsCountStr, ';');
        int neighborsCount = atoi(neighborsCountStr.c_str());
        std::vector<std::string> output;
        for(int i = 1; i <= neighborsCount; ++i) {
            getline(tokenizer, host, ';');
            getline(tokenizer, port, ';');
            output.push_back(host + ":" + port);
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
