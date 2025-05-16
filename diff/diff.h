/**
 * @author Dominik Milián
 * @Date 14.05.2025 
 */

#ifndef DIFF_DIFF_H
#define DIFF_DIFF_H

#include <unordered_map>
#include <optional>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <ranges>

class diff {
public:
    diff(char *file1, char *file2,
         const std::unordered_map<std::string, std::optional<std::string>> &options);

    ~diff();

    void run();

private:
    std::ifstream f1;
    std::ifstream f2;
    std::ostream *output;
    bool show_result;
    bool char_diff;

    void result_only();

    void with_print(bool use_words);

    template<class T>
    void process_lines(const std::string &line1, const std::string &line2, std::stringstream &line1_ss, std::stringstream &line2_ss, int count, const T &in1, const T &in2);

    std::vector<std::string> diff_split(const std::string &line);

    template<class T>
    std::vector<int> find_longest_common_subsequences(const T &line1, const T &line2, bool &lines_are_different);

    void print_colored(std::stringstream &ss, const std::string &line, int start, int end, int color);

    void print_colored(std::stringstream &ss, const std::vector<std::string> &words, int start, int end, int color);

    template<class T>
    std::vector<int>
    find_longest_common_subsequences(const T &line1, const T &line2, int m, int n,
                                     std::vector<std::vector<std::optional<std::vector<int>>>> &memo,
                                     bool &lines_are_different);
};


#endif //DIFF_DIFF_H
