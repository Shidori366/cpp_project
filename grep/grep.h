/**
 * @author Dominik Milián
 * @Date 13.05.2025 
 */

#ifndef GREP_GREP_H
#define GREP_GREP_H

#include <fstream>
#include <optional>
#include <unordered_map>
#include <iostream>
#include <regex>

class grep {
public:
    explicit grep(const std::unordered_map<std::string, std::optional<std::string>> &options);
    ~grep();

    void run();
    size_t match_pattern(const std::string &line, const std::regex &reg, size_t &matched_size);
    void printHighlighted(const std::string &line, const size_t &found_index, const size_t &matched_size);
private:
    std::istream *stream;
    int count;
    std::string pattern;
    bool highlight;
    bool regex;
};


#endif //GREP_GREP_H
