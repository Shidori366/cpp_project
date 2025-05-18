/**
 * @author Dominik Milián
 * @Date 17.05.2025 
 */

#include "find.h"

find::find(const std::unordered_map<std::string, std::optional<std::string>> &options) : regex(false), highlight(false), files_only(false),
                                                                                         count(-1), output(&std::cout) {
    auto path_option = options.find("p");
    auto name_option = options.find("n");
    auto count_option = options.find("c");
    auto output_option = options.find("o");

    if (path_option != options.end()) {
        this->start_path = path_option->second.value();
    }

    if (name_option != options.end()) {
        this->name = name_option->second.value();
    }

    if (count_option != options.end()) {
        this->count = std::stoi(count_option->second.value());
    }

    if (output_option != options.end()) {
        this->output = new std::ofstream(output_option->second.value());
        if (this->output->fail()) {
            throw std::runtime_error("failed to open output file.");
        }
    }

    if (options.find("fo") != options.end()) {
        this->files_only = true;
    }

    if (options.find("r") != options.end()) {
        this->regex = true;
    }

    if (options.find("h") != options.end()) {
        this->highlight = true;
    }
}

find::~find() {
    if (this->output == &std::cout) {
        return;
    }

    static_cast<std::ofstream *>(this->output)->close();
}

void find::run() {
    std::stack<std::filesystem::directory_entry> s;
    add_all_entries_to_stack(s, this->start_path);
    std::regex n_regex;
    size_t match_count = 0;

    if (this->regex) {
        n_regex = std::regex(this->name);
    }

    while(!s.empty()) {
        if (this->count != -1 && match_count == this->count) {
            break;
        }

        auto curr = s.top();
        s.pop();

        add_all_entries_to_stack(s, curr.path());

        if (files_only && curr.is_directory()) {
            continue;
        }

        std::string filename = curr.path().filename().string();
        size_t start_index = 0;
        size_t size = 0;
        if(match(filename, n_regex, start_index, size)) {
            size_t path_start = (curr.path().string().size() - filename.size()) + start_index;
            this->print(curr.path().string(), path_start, size);
            ++match_count;
        }
    }
}

void find::add_all_entries_to_stack(std::stack<std::filesystem::directory_entry> &s, const std::filesystem::path &path) {
    if (!is_directory(path)) {
        return;
    }

    std::filesystem::directory_iterator it(path);

    for (const auto &entry : it) {
        s.emplace(entry);
    }
}

bool find::match(const std::string &filename, const std::regex &n_regex, size_t &start_index, size_t &size) {
    if (!this->regex) {
        start_index = filename.find(this->name);
        size = this->name.size();
        return start_index != std::string::npos;
    }
    std::smatch smatch;
    std::regex_search(filename, smatch, n_regex);

    if (smatch.empty()) {
        return false;
    }

    start_index = smatch.position(0);
    size = smatch[0].str().size();

    return true;
}

void find::print(std::string &&path, size_t start, size_t size) {
    if (!this->highlight) {
        *this->output << path << std::endl;
        return;
    }

    *this->output << path.substr(0, start);

    *this->output << "\033[43;30m";
    *this->output << path.substr(start, size);
    *this->output << "\033[0m";

    *this->output << path.substr(start + size) << '\n';
}

