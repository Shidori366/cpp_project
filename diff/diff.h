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

class diff {
public:
    diff(char *file1, char *file2,
         const std::unordered_map<std::string, std::optional<std::string>> &options);

    ~diff();

    void run();
    void result_only();
    void print_diff();
    static bool print(int missing, char c1, char c2, std::stringstream &l1_diff, std::stringstream &l2_diff, int &color_set);
    static void setColor(std::stringstream &l1_diff, std::stringstream &l2_diff, int missing);
    static void resetColor(std::stringstream &l1_diff, std::stringstream &l2_diff);

private:
    std::ifstream f1;
    std::ifstream f2;
    std::ostream *output;
    bool show_result;
};


#endif //DIFF_DIFF_H
