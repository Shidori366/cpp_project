/**
 * @author Dominik Milián
 * @Date 05.04.2025
 */

#include "args_parser.h"

args_parser::args_parser(int argc, char **args, const std::vector<option_descriptor> &descriptors) {
    this->args.reserve(argc);

    for (int i = 0; i < argc; ++i) {
        this->args.emplace_back(args[i]);
    }

    this->process_args(descriptors);
}

void args_parser::process_args(const std::vector<option_descriptor> &descriptors) {
    for (const auto &descriptor: descriptors) {
        process_arg(descriptor);
    }
}

void args_parser::process_arg(const option_descriptor &descriptor) {
    int arg_index = this->find_arg(descriptor.name);

    if (arg_index == -1 && !descriptor.alternative_name.empty()) {
        arg_index = this->find_arg(descriptor.alternative_name);
    }

    if (arg_index == -1) {
        if (descriptor.required) {
            throw required_option_is_missing_exception();
        }

        return;
    }

    if (!descriptor.has_value) {
        this->parsed_options.insert({descriptor.name, std::nullopt});
        return;
    }

    int value_index = arg_index + 1;

    if (value_index == this->args.size() || !this->is_value(this->args[value_index])) {
        throw value_is_missing_exception();
    }

    this->parsed_options.insert({descriptor.name, this->args[arg_index + 1]});
}

int args_parser::find_arg(const std::string &arg_name) {
    int i = 0;

    for (const auto &arg: this->args) {
        if (this->arg_equals(arg, arg_name)) {
            return i;
        }

        ++i;
    }

    return -1;
}

bool args_parser::arg_equals(const std::string &arg, const std::string &option) {
    if ((arg.size() - 1) != option.size()) {
        return false;
    }

    for (int i = 1; i < arg.size(); ++i) {
        if (arg[i] != option[i-1]) {
            return false;
        }
    }

    return true;
}

bool args_parser::is_value(const std::string &str) {
    return !str.empty() && str[0] != '-';
}

const std::unordered_map<std::string, std::optional<std::string>> &args_parser::get_parsed_options() const {
    return this->parsed_options;
}






