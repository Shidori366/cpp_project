#include <iostream>
#include "args_parser.h"
#include "diff.h"

int main(int argc, char **args) {
    if (argc < 3) {
        std::cerr << "wrong number of parameters" << std::endl;
        return EXIT_FAILURE;
    }

    args_parser args_parser(argc, args, {
            {"r", "result",   false, false},
            {"o", "output", true, false},
    });
    const auto &parsed_options = args_parser.get_parsed_options();

    diff diff(args[1], args[2], parsed_options);
    diff.run();

    return EXIT_SUCCESS;
}
