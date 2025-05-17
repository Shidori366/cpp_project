/**
 * @author Dominik Milián
 * @Date 17.05.2025 
 */

#ifndef FIND_FIND_H
#define FIND_FIND_H

#include <iostream>
#include <unordered_map>
#include <optional>
#include <string>
#include <fstream>
#include <filesystem>
#include <stack>
#include <regex>

class find {
public:
    find(const std::unordered_map<std::string, std::optional<std::string>> &options);

    void run();
private:
    std::string start_path;
    std::string name;
    bool regex;
    bool highlight;
    bool files_only;
    int count;
    std::ostream *output;

    void add_all_entries_to_stack(std::stack<std::filesystem::directory_entry> &s, const std::filesystem::path &path);
    bool match(const std::string &filename, const std::regex &n_regex, size_t &start_index, size_t &size);
    void print(std::string &&path, size_t start, size_t size);
};


#endif //FIND_FIND_H
