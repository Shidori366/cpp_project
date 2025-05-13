/**
 * @author Dominik Milián
 * @Date 13.05.2025 
 */

#include "grep.h"

grep::grep(const std::unordered_map<std::string, std::optional<std::string>> &options) : stream(&std::cin), count(-1),
                                                                                         highlight(false) {
    auto file_option = options.find("f");
    if (file_option != options.end()) {
        this->stream = new std::ifstream(file_option->second.value());
    }
    this->pattern = options.at("p").value();

    if (options.find("r") != options.end()) {
        this->regex = true;
    }
    if (options.find("h") != options.end()) {
        this->highlight = true;
    }

    auto count_option = options.find("c");
    if (count_option != options.end()) {
        this->count = std::stoi(count_option->second.value());
    }
}

grep::~grep() {
    if (this->stream == &std::cin) {
        return;
    }

    static_cast<std::ifstream *>(this->stream)->close();
    delete this->stream;
}


void grep::run() {
    std::string line;
    int currentCount = 0;
    std::regex reg;

    if (this->regex) {
        reg = std::regex(this->pattern);
    }

    while (std::getline(*stream, line)) {
        size_t matched_size;
        size_t found_index = match_pattern(line, reg, matched_size);
        if (found_index == std::string::npos) {
            continue;
        }
        if (!highlight) {
            std::cout << line << '\n';
        } else {
            printHighlighted(line, found_index, matched_size);
        }
        ++currentCount;
        if (this->count != -1 && currentCount == this->count) {
            break;
        }
    }
}

size_t grep::match_pattern(const std::string &line, const std::regex &reg, size_t &matched_size) {
    if (!this->regex) {
        matched_size = this->pattern.size();
        std::size_t found_index = line.find(this->pattern);
        return found_index;
    }
    std::smatch match;
    std::regex_search(line, match, reg);

    if (match.empty()) {
        return std::string::npos;
    }

    matched_size = match[0].str().size();
    return match.position(0);
}

void grep::printHighlighted(const std::string &line, const size_t &found_index, const size_t &matched_size) {
    std::cout << line.substr(0, found_index);

    std::cout << "\033[43;30m";
    std::cout << line.substr(found_index, matched_size);
    std::cout << "\033[0m";

    std::cout << line.substr(found_index + matched_size) << '\n';
}
