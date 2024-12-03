#include <chrono>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <optional>
#include <utility>
#include <vector>
#include <algorithm>

typedef std::vector<int> report_type;
typedef std::vector<report_type> report_list;

const report_list parse_data(std::string data) {
    report_list result;
    size_t pos = 0;
    while ((pos = data.find("\n")) != std::string::npos) {
        auto line = data.substr(0, pos);
        report_type report;

        auto line_stream = std::istringstream(line);
        int value;
        while (!line_stream.eof()) {
            line_stream >> value;
            report.emplace_back(value);
        }
        result.push_back(report);
        data.erase(0, pos + 1);
    }

    return result;
}

int64_t part1(std::string data) {
    report_list parsed_data = parse_data(data);

    int64_t total_safe = 0;
    for (auto report : parsed_data) {
        auto value = report.front();
        bool safe = true;
        std::optional<bool> increasing = std::nullopt;

        for (auto it = report.cbegin()+1; it != report.cend(); it++) {
            auto value_next = *it;
            if (!increasing) {
                increasing = (value_next > value);
            }
            if ((value_next > value) != *increasing) {
                safe = false;
                break;
            }

            int diff = abs(value_next - value);
            if (diff < 1 || diff > 3) {
                safe = false;
                break;
            }
            value = value_next;
        }
        // std::cout << (safe ? " Safe : " : "Unsafe: ");
        // std::for_each(report.cbegin(), report.cend(), [](int v) {std::cout << v << ", ";});
        // std::cout << std::endl;

        total_safe += safe;
    }

    return total_safe;
}

int64_t part2(std::string data) {
    report_list parsed_data = parse_data(data);

    auto unsafe_step = [](int p, int n, bool increasing) {
        if ((n > p) != increasing) {
            return true;
        }
        int diff = abs(n - p);
        if (diff < 1 || diff > 3) {
            return true;
        }
        return false;
    };

    int64_t total_safe = 0;
    for (auto report : parsed_data) {
        bool any_safe = false;
        for (int skip = 0; skip < report.size(); skip++) {
            auto value = report[(skip == 0)];
            bool safe = true;
            std::optional<bool> increasing = std::nullopt;

            for (auto i = 1; i + (i >= skip) < report.size(); i++) {
                auto value_next = report[i + (i >= skip)];
                if (!increasing) {
                    increasing = (value_next > value);
                }
                if ((value_next > value) != *increasing) {
                    safe = false;
                    break;
                }

                int diff = abs(value_next - value);
                if (diff < 1 || diff > 3) {
                    safe = false;
                    break;
                }
                value = value_next;
            }
            // std::cout << (safe ? " Safe : " : "Unsafe: ");
            // std::for_each(report.cbegin(), report.cend(), [](int v) {std::cout << v << ", ";});
            // std::cout << std::endl;
            if (safe) {
                any_safe = true;
                break;
            }
        }

        total_safe += any_safe;
    }

    return total_safe;
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
        std::cout << "Advent of Code 02" << std::endl;
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