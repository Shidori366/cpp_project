//
// Created by shidori on 4/7/25.
//

#include "split.h"


split::split(const std::unordered_map<std::string, std::optional<std::string>> &options)
        : suffix_length(2), count(0), lines(0), bytes(0) {
    this->input_file_name = options.at("f").value();
    this->input_file = std::ifstream(input_file_name);

    if (this->input_file.fail()) {
        throw std::runtime_error("Could not open file");
    }

    // TODO: check for negative values
    if (options.find("s") != options.end()) {
        this->suffix_length = std::stoul(options.at("s").value());
    }

    if (options.find("c") != options.end()) {
        this->count = std::stoul(options.at("c").value());
    }

    if (options.find("l") != options.end()) {
        this->lines = std::stoul(options.at("l").value());
    }

    if (options.find("b") != options.end()) {
        this->bytes = std::stoul(options.at("b").value());
    }

    this->validate_args();
}

split::~split() {
    this->input_file.close();
}

void split::run() {
    if (count != 0) {
        this->split_count();
        return;
    }

    if (lines != 0) {
        this->split_lines();
        return;
    }

    this->split_bytes();
}

void split::validate_args() {
    if (count == 0 && lines == 0 && bytes == 0) {
        throw std::runtime_error("Invalid arguments");
    }
    if (pow(26, this->suffix_length) < this->count_number_of_parts()) {
        throw std::runtime_error("Number of file parts is greater than number of possible suffixes");
    }
}

size_t split::count_number_of_parts() {
    if (this->count != 0) {
        return this->count;
    }
    if (this->lines != 0) {
        return this->number_of_lines() / this->lines;
    }
    return this->number_of_bytes() / this->bytes;
}

size_t split::number_of_lines() {
    size_t count_l = 0;
    std::string line;

    while (std::getline(this->input_file, line)) {
        ++count_l;
    }

    this->reset_input_position();

    return count_l;
}

size_t split::number_of_bytes() {
    size_t count_b = 0;

    while (true) {
        this->input_file.get();

        if (this->input_file.eof()) {
            break;
        }
        ++count_b;
    }

    this->reset_input_position();

    return count_b;
}

void split::reset_input_position() {
    this->input_file.clear();
    this->input_file.seekg(0, std::ios::beg);
}

void split::split_count() {
    const size_t part_line_count = this->number_of_lines() / this->count;
    size_t current_suffix = 0;
    size_t current_count = 0;

    std::string current_line;

    while (!this->input_file.eof() && current_count != this->count) {
        std::ofstream current_part = std::ofstream(this->input_file_name + std::to_string(current_suffix),
                                                   std::ios::out);
        size_t current_line_count = 0;

        while (std::getline(this->input_file, current_line)) {
            current_part << current_line << '\n';
            ++current_line_count;
            if (current_line_count == part_line_count && (current_count != this->count - 1)) {
                break;
            }
        }
        current_part.close();
        ++current_count;
        ++current_suffix;
    }
}

void split::split_lines() {
    std::string current_line;
    size_t current_suffix = 0;

    while (!input_file.eof()) {
        size_t current_line_count = 0;
        std::ofstream current_part = std::ofstream(this->input_file_name + std::to_string(current_suffix),
                                                   std::ios::out);

        while (std::getline(this->input_file, current_line)) {
            current_part << current_line << '\n';
            ++current_line_count;
            if (current_line_count == this->lines) {
                break;
            }
        }

        ++current_suffix;
        current_part.close();
    }
}

void split::split_bytes() {
    size_t current_suffix = 0;

    while (!this->input_file.eof()) {
        size_t current_bytes_count = 0;
        std::ofstream current_part = std::ofstream(this->input_file_name + std::to_string(current_suffix),
                                                   std::ios::out);

        while (true) {
            char c = static_cast<char>(this->input_file.get());

            if (this->input_file.eof()) {
                break;
            }

            current_part << c;
            ++current_bytes_count;

            if (current_bytes_count == this->bytes) {
                break;
            }
        }

        ++current_suffix;
        current_part.close();
    }
}


