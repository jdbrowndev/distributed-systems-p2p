#ifndef PRINT_HELPER_H_
#define PRINT_HELPER_H_ 

#include <map>
#include <string>

namespace brown {
    class print_helper { 
    private:
        const static int COMMAND_LIST_INDENT = 3;
        const static int COMMAND_LIST_SPACING = 8;
        const static int TITLE_PADDING = 5;
    public:
        void printWelcomeMessage();
        void printListUsage();
        void printSelectUsage();
        void printFileUsage();
        void printShareUsage();
        void printDecoratedTitle(std::string title);
        void printCommands(std::map<std::string, std::string>& commands);
    };
} /* namespace brown */

#endif
