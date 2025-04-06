/**
 * @author Dominik Milián
 * @Date 05.04.2025 
 */

#ifndef ARGS_PARSER_OPTION_DESCRIPTOR_H
#define ARGS_PARSER_OPTION_DESCRIPTOR_H

#include <string>

struct option_descriptor {
    std::string name;
    std::string alternative_name;

    bool has_value;
    bool required;
};

#endif //ARGS_PARSER_OPTION_DESCRIPTOR_H
