#include <iostream>
#include <sstream>
#include "print_helper.h"

namespace brown {
    void print_helper::printWelcomeMessage() {
        std::cout << "Client: CLI launched. Type 'help' for a list of commands." << std::endl;
    }

    void print_helper::printListUsage() {
        std::cout << "Usage: list [-s]" << std::endl;
        std::cout << "   -s = flag to list all nodes in system." << std::endl;
        std::cout << "Without -s, only known neighbors will be printed." << std::endl;
    }

    void print_helper::printSelectUsage() {
        std::cout << "Usage: select neighbor-id" << std::endl;
        std::cout << "   neighbor-id = ID of a neighbor to select (see 'list' command)." << std::endl;
        std::cout << "A ping request will be sent to the neighbor associated with the selected ID." << 
            std::endl;
    }

    void print_helper::printFileUsage() {
        std::cout << "Usage: file [-s] filename" << std::endl;
        std::cout << "   -s = flag to search all nodes in system." << std::endl;
        std::cout << "   filename = File to search for." << std::endl;
        std::cout << "File search will default to the selected neighbor if -s is not given." << std::endl;
    }

    void print_helper::printShareUsage() {
        std::cout << "Usage: share" << std::endl;
        std::cout << "Share request defaults to a selected neighbor." << std::endl;
        std::cout << "A neighbor must be selected for the command to work correctly." << std::endl;
    }

    void print_helper::printDecoratedTitle(std::string title) {
        const int titleDecorLength = title.length() + TITLE_PADDING * 2;
        std::stringstream outputStream;
        outputStream << "\n";
        for(int i = 1; i <= titleDecorLength; ++i) {
            outputStream << "=";
        }
        outputStream << "\n";
        for(int i = 1; i <= TITLE_PADDING; ++i) {
            outputStream << " ";
        }
        outputStream << title << "\n";
        for(int i = 1; i <= titleDecorLength; ++i) {
            outputStream << "=";
        }
        std::cout << outputStream.str() << std::endl;
    }

    void print_helper::printCommands(std::map<std::string, std::string>& commands) {
        std::stringstream outputStream;
        outputStream << "Commands:" << std::endl;
        for(std::map<std::string, std::string>::iterator it = commands.begin(); it != commands.end(); ++it) {
            for(int spaces = 1; spaces <= COMMAND_LIST_INDENT; ++spaces) {
                outputStream << " ";
            }
            outputStream << it->first;
            for(int spaces = 1; spaces <= COMMAND_LIST_SPACING - (signed)it->first.length(); ++spaces) {
                outputStream << " ";
            }
            outputStream << it->second << std::endl;
        }
        std::cout << outputStream.str();
    }
} /* namespace brown */
