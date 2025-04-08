//
// Created by shidori on 4/7/25.
//

#ifndef SPLIT_SPLIT_H
#define SPLIT_SPLIT_H

#include <unordered_map>
#include <string>
#include <optional>
#include <fstream>
#include <cmath>

class split {
public:
    explicit split(const std::unordered_map<std::string, std::optional<std::string>> &options);
    ~split();
    void run();
private:
    std::ifstream input_file;
    std::string input_file_name;

    size_t suffix_length;
    size_t count;
    size_t lines;
    size_t bytes;

    void validate_args();
    size_t count_number_of_parts();
    size_t number_of_lines();
    size_t number_of_bytes();

    void split_count();
    void split_lines();
    void split_bytes();

    void reset_input_position();
};


#endif //SPLIT_SPLIT_H
