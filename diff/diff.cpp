/**
 * @author Dominik Milián
 * @Date 14.05.2025 
 */

#include "diff.h"

diff::diff(char *file1, char *file2,
           const std::unordered_map<std::string, std::optional<std::string>> &options) : show_result(false),
                                                                                         char_diff(false),
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

    if (options.find("c") != options.end()) {
        this->char_diff = true;
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

    if (char_diff) {
        with_print(false);
    } else {
        with_print(true);
    }
}

void diff::result_only() {
    std::string line1;
    std::string line2;

    bool f1_lines;
    bool f2_lines;

    do {
        f1_lines = !!std::getline(this->f1, line1);
        f2_lines = !!std::getline(this->f2, line2);

        if (!std::equal(line1.begin(), line1.end(), line2.begin(), line2.end())) {
            std::cout << "false" << std::endl;
            return;
        }
    } while (f1_lines || f2_lines);

    std::cout << "true" << std::endl;
}

void diff::with_print(bool use_words) {
    std::string line1;
    std::string line2;

    int count = 1;

    while (true) {
        bool l1_loaded = !!std::getline(this->f1, line1);
        bool l2_loaded = !!std::getline(this->f2, line2);

        if (!l1_loaded && !l2_loaded) {
            return;
        }

        std::stringstream line1_ss;
        std::stringstream line2_ss;
        if (!l1_loaded || !l2_loaded) {
            const std::string &line = l2_loaded ? line2 : line1;
            const int color = l2_loaded ? 1 : 0;
            const size_t end = l2_loaded ? line2.size() : line1.size();
            std::stringstream &ss = l2_loaded ? line2_ss : line1_ss;
            print_colored(ss, line, 0, static_cast<int>(end), color);

            *this->output << count << '\t' << ss.str() << '\n';
            ++count;
            continue;
        }

        if (use_words) {
            auto split1 = diff_split(line1);
            auto split2 = diff_split(line2);
            process_lines(line1, line2, line1_ss, line2_ss, count, split1, split2);
        } else {
            process_lines(line1, line2, line1_ss, line2_ss, count, line1, line2);
        }

        ++count;
    }
}

template<class T>
std::vector<int> diff::find_longest_common_subsequences(const T &line1, const T &line2, bool &lines_are_different) {
    int m = static_cast<int>(line1.size());
    int n = static_cast<int>(line2.size());
    lines_are_different = false;
    std::vector<std::vector<std::optional<std::vector<int>>>> memo(m + 1,
                                                                   std::vector<std::optional<std::vector<int>>>(n + 1,
                                                                                                                std::nullopt));
    return find_longest_common_subsequences(line1, line2, m, n, memo, lines_are_different);
}

template<class T>
std::vector<int> diff::find_longest_common_subsequences(const T &line1, const T &line2, int m, int n,
                                                        std::vector<std::vector<std::optional<std::vector<int>>>> &memo,
                                                        bool &lines_are_different) {
    if (m == 0 || n == 0) {
        return {};
    }

    if (memo[m][n] != std::nullopt) {
        return memo[m][n].value();
    }

    if (line1[m - 1] == line2[n - 1]) {
        std::vector<int> current = {n - 1, m - 1};
        auto next_common = find_longest_common_subsequences(line1, line2, m - 1, n - 1, memo, lines_are_different);
        current.insert(current.end(), next_common.begin(), next_common.end());

        memo[m][n] = std::optional(current);
        return current;
    }

    lines_are_different = true;
    auto next_common_n_less = find_longest_common_subsequences(line1, line2, m, n - 1, memo, lines_are_different);
    auto next_common_m_less = find_longest_common_subsequences(line1, line2, m - 1, n, memo, lines_are_different);

    memo[m][n] = std::optional(
            next_common_m_less.size() > next_common_n_less.size() ? next_common_m_less : next_common_n_less);
    return memo[m][n].value();
}

std::vector<std::string> diff::diff_split(const std::string &line) {
    std::vector<std::string> split;
    std::string current;

    for (const auto &item: line) {
        if (item == ' ') {
            split.emplace_back(current);
            split.emplace_back(" ");
            current.erase();
            continue;
        }
        current += item;
    }
    split.emplace_back(current);

    return split;
}

void
diff::print_colored(std::stringstream &ss, const std::string &line, const int start, const int end, const int color) {
    ss << (color == 0 ? "\033[31m" : "\033[32m");

    ss << line.substr(start, end - start);

    ss << "\033[0m";
}

void diff::print_colored(std::stringstream &ss, const std::vector<std::string> &words, int start, int end, int color) {
    ss << (color == 0 ? "\033[31m" : "\033[32m");

    for (size_t i = start; i < end; ++i) {
        ss << words[i];
    }

    ss << "\033[0m";
}

template<class T>
void diff::process_lines(const std::string &line1, const std::string &line2, std::stringstream &line1_ss, std::stringstream &line2_ss, int count, const T &in1, const T &in2) {
    bool lines_are_different;
    auto common = find_longest_common_subsequences(in1, in2, lines_are_different);

    if (!lines_are_different) {
        return;
    }

    if (common.empty()) {
        print_colored(line1_ss, line1, 0, static_cast<int>(line1.size()), 0);
        print_colored(line2_ss, line2, 0, static_cast<int>(line2.size()), 1);

        *this->output << count << '\t' << line1_ss.str() << '\n';
        *this->output << count << '\t' << line2_ss.str() << "\n\n";
        return;
    }

    std::ranges::reverse_view rv{common};

    if (rv[0] != 0) {
        print_colored(line1_ss, in1, 0, rv[0], 0);
    }
    if (rv[1] != 0) {
        print_colored(line2_ss, in2, 0, rv[1], 1);
    }

    int i;
    for (i = 0; i <= common.size() - 2; i += 2) {
        line1_ss << in1[rv[i]];
        if (i < common.size() - 2 && (rv[i + 2] - rv[i]) > 1) {
            print_colored(line1_ss, in1, rv[i] + 1, rv[i + 2], 0);
        }
    }
    if (rv[i - 2] < in1.size() - 1) {
        print_colored(line1_ss, in1, rv[i - 2] + 1, static_cast<int>(in1.size()), 0);
    }

    for (i = 1; i <= common.size() - 1; i += 2) {
        line2_ss << in2[rv[i]];
        if (i < common.size() - 1 && (rv[i + 2] - rv[i]) > 1) {
            print_colored(line2_ss, in2, rv[i] + 1, rv[i + 2], 1);
        }
    }
    if (rv[i - 2] < line2.size() - 1) {
        print_colored(line2_ss, in2, rv[i - 2] + 1, static_cast<int>(in2.size()), 1);
    }

    *this->output << count << '\t' << line1_ss.str() << '\n';
    *this->output << count << '\t' << line2_ss.str() << "\n\n";
}
