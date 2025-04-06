/**
 * @author Dominik Milián
 * @Date 05.04.2025 
 */

#include "include/args_parser.h"

int main(int argc, char **args) {
    std::vector<option_descriptor> options {
            {"f", "file", true, true},
            {"b", "", false, false},
            {"l", "", false, false},
            {"w", "", false, false},
            {"c", "", false, false},
            {"o", "output", true, false},
    };

    args_parser argsParser(argc, args, options);

    return 0;
}