#include "args_parser.h"
#include "find.h"

int main(int argc, char **args) {
    args_parser args_parser(argc, args, {
            {"p", "path",   true, true},
            {"n", "name", true, true},
            {"fo", "fileonly", false, false},
            {"r", "regex", false, false},
            {"h", "highlight", false, false},
            {"c", "count", true, false},
            {"o", "output", true, false},
    });

    find find(args_parser.get_parsed_options());
    find.run();

    return 0;
}
