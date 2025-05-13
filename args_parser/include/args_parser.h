/**
 * @author Dominik Milián
 * @Date 05.04.2025
 */

#ifndef ARGS_PARSER_ARGS_PARSER_H
#define ARGS_PARSER_ARGS_PARSER_H

#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <optional>

#include "option_descriptor.h"

#include "exceptions/value_is_missing_exception.h"
#include "exceptions/required_option_is_missing_exception.h"

class args_parser {
public:
    explicit args_parser(int argc, char **args, const std::vector<option_descriptor> &descriptors);

    const std::unordered_map<std::string, std::optional<std::string>> &get_parsed_options() const;

private:
    std::vector<std::string> args;
    std::unordered_map<std::string, std::optional<std::string>> parsed_options;

    void process_args(const std::vector<option_descriptor> &descriptors);
    void process_arg(const option_descriptor &descriptor);
    int find_arg(const std::string &arg_name, bool alternative);
    bool arg_equals(const std::string &arg, const std::string &option, bool alternative);
    bool is_value(const std::string &str);
};

#endif //ARGS_PARSER_ARGS_PARSER_H
