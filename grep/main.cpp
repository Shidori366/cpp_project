#include <iostream>
#include "args_parser.h"
#include "grep.h"

int main(int argc, char **args) {
    args_parser args_parser(argc, args, {
            {"f", "file",   true, false},
            {"p", "pattern", true, true},
            {"r", "regex",  false, false},
            {"h", "highlight",  false, false},
            {"c", "count",  true, false},
    });
    const auto &parsed_options = args_parser.get_parsed_options();

    grep grep(parsed_options);
    grep.run();

    return EXIT_SUCCESS;
}
