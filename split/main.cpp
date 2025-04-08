#include "args_parser.h"
#include "split.h"
#include <cmath>

int main(int argc, char **args) {
    args_parser args_parser(argc, args, {
            {"f", "file", true, true},
            {"s", "suffix", true, false},
            {"c", "count", true, false},
            {"l", "lines", true, false},
            {"b", "bytes", true, false},
    });
    const auto &parsed_options = args_parser.get_parsed_options();

    split split(parsed_options);
    split.run();

    return EXIT_SUCCESS;
}
