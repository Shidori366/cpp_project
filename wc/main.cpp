#include <iostream>
#include <fstream>
#include <args_parser.h>
#include <cstring>
#include <stdint.h>

size_t num_of_bytes(const uint8_t first_byte) {
    if (((first_byte & 0x80) >> 7) == 0b0) {
        return 1;
    }
    if (((first_byte & 0xE0) >> 5) == 0b110) {
        return 2;
    }
    if (((first_byte & 0xF0) >> 4) == 0b1110) {
        return 3;
    }

    return 4;
}

void skip_n_bytes(size_t n, std::ifstream &in) {
    for (int i = 0; i < n; ++i) {
        in.get();
    }
}

void reset_in_file_position(std::ifstream &in) {
    in.clear();
    in.seekg(0, std::ios::beg);
}

size_t get_char_count(std::ifstream &in) {
    reset_in_file_position(in);
    size_t count = 0;

    while(true) {
        uint8_t c = in.get();
        if (in.eof()) {
            break;
        }
        size_t byte_count = num_of_bytes(c);
        skip_n_bytes(byte_count - 1, in);
        ++count;
    }

    return count;
}

size_t get_byte_count(std::ifstream &in) {
    reset_in_file_position(in);
    size_t count = 0;

    while(true) {
        in.get();
        if (in.eof()) {
            break;
        }
        ++count;
    }

    return count;
}

size_t get_line_count(std::ifstream &in) {
    reset_in_file_position(in);
    size_t count = 0;
    std::string line;

    while (std::getline(in, line)) {
        ++count;
    }

    return count;
}

size_t get_word_count(std::ifstream &in) {
    reset_in_file_position(in);
    size_t count = 0;
    std::string line;

    while (std::getline(in, line)) {
        bool processing_word = false;

        for (const auto &item: line) {
            bool is_space = std::isspace(item);

            if (!is_space && !processing_word) {
                processing_word = true;
                ++count;
                continue;
            }

            if (is_space) {
                processing_word = false;
            }
        }
    }

    return count;
}

void clear_out_file(const std::string &path) {
    std::ofstream out(path);
    out.close();
}

void write_result(const std::unordered_map<std::string, std::optional<std::string>> &parsed_options, const size_t result, const std::string &message) {
    std::ostream *result_stream = &std::cout;
    std::ofstream out;

    if (parsed_options.find("o") != parsed_options.end()) {
        out = std::ofstream(parsed_options.at("o").value(), std::ios::app);

        if (!out.fail()) {
            result_stream = &out;
        }
    }

    *result_stream << message << result << std::endl;

    if (result_stream == &out) {
        out.close();
    }
}

int main(int argc, char **args) {
    args_parser args_parser(argc, args, {
            {"f", "file", true, true},
            {"o", "output", true, false},
            {"b", "", false, false},
            {"l", "", false, false},
            {"w", "", false, false},
            {"c", "", false, false},
    });

    const auto &parsed_options = args_parser.get_parsed_options();
    auto input_file_name = parsed_options.at("f").value();

    std::ifstream in(input_file_name);

    if (in.fail()) {
        std::cerr << "File " << input_file_name << " could not be opened" << std::endl;
        std::cerr << "Error: " << std::strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    if (parsed_options.find("o") != parsed_options.end()) {
        clear_out_file(parsed_options.at("o").value());
    }

    if (parsed_options.find("l") != parsed_options.end()) {
        write_result(parsed_options, get_line_count(in), "Line count: ");
    }

    if (parsed_options.find("w") != parsed_options.end()) {
        write_result(parsed_options, get_word_count(in), "Word count: ");
    }

    if (parsed_options.find("c") != parsed_options.end()) {
        write_result(parsed_options, get_char_count(in), "Character count: ");
    }

    if (parsed_options.find("b") != parsed_options.end()) {
        write_result(parsed_options, get_byte_count(in), "Byte count: ");
    }

    in.close();

    return EXIT_SUCCESS;
}
