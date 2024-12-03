#include <chrono>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <filesystem>
#include <string>
#include <optional>
#include <utility>
#include <vector>
#include <algorithm>

typedef std::vector<int> report_type;
typedef std::vector<report_type> report_list;

int64_t part1(std::string data) {
    std::regex mul_regex("mul\\((\\d+),(\\d+)\\)");
    auto matches_begin = std::sregex_iterator(data.begin(), data.end(), mul_regex);
    auto matches_end = std::sregex_iterator();

    int64_t total = 0;
    for (auto it = matches_begin; it != matches_end; ++it) {
        std::smatch match = *it;
        int multiplied = atoi(match[1].str().c_str()) * atoi(match[2].str().c_str());
        total += multiplied;
        std::cout << match.str() << " = " << multiplied << std::endl;
    }    
    return total;
}

enum MatchType {
    MUL = 0,
    ENABLE = 1,
    DISABLE = 2,
};

struct MatchPosition {
    size_t pos;
    MatchType match_type;
    int value;
    MatchPosition(size_t pos, MatchType match_type, int value)
    : pos(pos), match_type(match_type), value(value) {
    }
};

int64_t part2(std::string data) {
    std::vector<MatchPosition> operations;
    constexpr const char* regexes[] = {
        "mul\\((\\d+),(\\d+)\\)",
        "do\\(\\)",
        "don't\\(\\)",
    };
    for (int regex_type = 0; regex_type < 3; ++regex_type) {
        auto regex = std::regex(regexes[regex_type]);

        auto matches_begin = std::sregex_iterator(data.begin(), data.end(), regex);
        auto matches_end = std::sregex_iterator();
        for (auto it = matches_begin; it != matches_end; ++it) {
            std::smatch match = *it;
            int value = 0;
            if (regex_type == MatchType::MUL) {
                value = atoi(match[1].str().c_str()) * atoi(match[2].str().c_str());
            }
            operations.emplace_back(
                match.position(),
                MatchType(regex_type),
                value
            );
        }  
    }
    std::sort(operations.begin(), operations.end(), [](MatchPosition a, MatchPosition b){return a.pos < b.pos;});

    int64_t total = 0;
    bool is_enabled = true;
    for (auto op : operations) {
        switch (op.match_type) {
            case MatchType::MUL:
                total += op.value * is_enabled;
            break;
            case MatchType::ENABLE:
                is_enabled = true;
            break;
            case MatchType::DISABLE:
                is_enabled = false;
            break;
        }
        std::cout << op.pos << ": " << op.match_type << " (" << op.value << ")" << std::endl;
    }    
    return total;
}

//////////////////////////////////////////////////////////////////////////////

namespace fs = std::filesystem;

std::optional<std::string> load_data(std::string path_to_input) {
    const fs::path p = path_to_input;
    if (!fs::exists(p)) {
        std::cerr << "Couldn't open input file: " << path_to_input;
        return std::nullopt;
    }
    auto file_handle = std::ifstream(p);
    std::ostringstream file_stream;
    file_stream << file_handle.rdbuf();
    return file_stream.str();
}

int main(int argc, const char* argv[]) {
    if (argc < 2 || argc > 3) {
        std::cout << "Advent of Code 03" << std::endl;
        std::cout << ">   -1 [path_to_input]: run part 1" << std::endl;
        std::cout << ">   -2 [path_to_input]: run part 2" << std::endl;
        return 1;
    }

    const auto path_to_input = (argc == 3) ? argv[2] : "input.txt";
    const auto data = load_data(path_to_input);
    if (!data) {
        return 2;
    }

    if (std::string(argv[1]) == "-1") {
        auto time_start = std::chrono::high_resolution_clock::now();
        const auto result = part1(*data);
        auto time_stop = std::chrono::high_resolution_clock::now();
        auto duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(time_stop - time_start);
        std::cout << "Advent of Code 01.1: " << result << " (" << duration_ns.count() << " ns)" << std::endl;
    }

    if (std::string(argv[1]) == "-2") {
        auto time_start = std::chrono::high_resolution_clock::now();
        const auto result = part2(*data);
        auto time_stop = std::chrono::high_resolution_clock::now();
        auto duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(time_stop - time_start);
        std::cout << "Advent of Code 01.2: " << result << " (" << duration_ns.count() << " ns)" << std::endl;
    }

    return 0;
}