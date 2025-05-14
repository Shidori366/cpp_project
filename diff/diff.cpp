/**
 * @author Dominik Milián
 * @Date 14.05.2025 
 */

#include "diff.h"

diff::diff(char *file1, char *file2,
           const std::unordered_map<std::string, std::optional<std::string>> &options) : show_result(false),
                                                                                         output(&std::cout) {
    this->f1 = std::ifstream(file1);
    if (this->f1.fail()) {
        throw std::runtime_error("failed to open first file.");
    }
    this->f2 = std::ifstream(file2);
    if (this->f2.fail()) {
        throw std::runtime_error("failed to open second file.");
    }

    if (options.find("r") != options.end()) {
        this->show_result = true;
    }

    auto output_option = options.find("o");
    if (output_option != options.end()) {
        this->output = new std::ofstream(output_option->second.value());
    }
}

diff::~diff() {
    if (this->output == &std::cout) {
        return;
    }

    static_cast<std::ofstream *>(this->output)->close();
}


void diff::run() {
    if (show_result) {
        result_only();
        return;
    }

    print_diff();
}

void diff::result_only() {
    std::string line1;
    std::string line2;

    bool f1_lines;
    bool f2_lines;

    do {
        f1_lines = !!std::getline(this->f1, line1);
        f2_lines = !!std::getline(this->f2, line2);

        if (!std::equal(line1.begin(), line1.end(),line2.begin(), line2.end())) {
            std::cout << "false" << std::endl;
            return;
        }
    } while (f1_lines || f2_lines);

    std::cout << "true" << std::endl;
}

void diff::print_diff() {
    std::string line1;
    std::string line2;

    bool f1_line;
    bool f2_line;

    size_t line_num = 0;

    do {
        f1_line = !!std::getline(this->f1, line1);
        f2_line = !!std::getline(this->f2, line2);

        size_t bigger_size = line1.size() > line2.size() ? line1.size() : line2.size();
        std::stringstream l1_diff;
        std::stringstream l2_diff;
        int color_set = -1;
        bool change = false;

        for (int i = 0; i < bigger_size; ++i) {
            int missing = 0;
            if (i >= line1.size()) {
                missing = 1;
            } else if (i >= line2.size()) {
                missing = 2;
            }
            char c1 = missing != 1 ? line1[i] : c1;
            char c2 = missing != 2 ? line2[i] : c2;

            if(print(missing, c1, c2, l1_diff, l2_diff, color_set)) {
                change = true;
            }
        }

        resetColor(l1_diff, l2_diff);
        if (change) {
            *this->output << line_num << '\t' << l1_diff.str() << '\n';
            *this->output << line_num << '\t' << l2_diff.str() << "\n\n";
        }
        ++line_num;
    } while (f1_line || f2_line);
}

bool diff::print(const int missing, const char c1, const char c2, std::stringstream &l1_diff, std::stringstream &l2_diff, int &color_set) {
    bool equals = c1 == c2;
    if (missing == 0) {
        if (equals && color_set != -1) {
            color_set = -1;
            resetColor(l1_diff, l2_diff);
        } else if (!equals && color_set != missing) {
            color_set = missing;
            setColor(l1_diff, l2_diff, missing);
        }

        l1_diff << c1;
        l2_diff << c2;
        return !equals;
    }

    if (color_set != missing) {
        color_set = missing;
        setColor(l1_diff, l2_diff, missing);
    }

    if (missing == 1) {
        l2_diff << c2;
    } else {
        l1_diff << c1;
    }
    return true;
}

void diff::setColor(std::stringstream &l1_diff, std::stringstream &l2_diff, const int missing) {
    if (missing == 0) {
        l1_diff << "\033[31m";
        l2_diff << "\033[31m";
    } else if (missing == 1) {
        l1_diff << "\033[31m";
        l2_diff << "\033[32m";
    } else {
        l1_diff << "\033[32m";
        l2_diff << "\033[31m";
    }
}

void diff::resetColor(std::stringstream &l1_diff, std::stringstream &l2_diff) {
    l1_diff << "\033[0m";
    l2_diff << "\033[0m";
}


