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

typedef std::vector<int> elf_list;
typedef std::pair<const elf_list, const elf_list> elf_plan;

const elf_plan parse_data(std::string data) {
    elf_list a;
    elf_list b;

    auto data_stream = std::istringstream(data);
    int loc_a, loc_b;
    data_stream >> loc_a >> loc_b;
    while (!data_stream.eof()) {
        a.emplace_back(loc_a);
        b.emplace_back(loc_b);
        data_stream >> loc_a >> loc_b;
    }

    return elf_plan(a, b);
}

int64_t part1(std::string data) {
    elf_plan parsed_data = parse_data(data);
    elf_list a = parsed_data.first;
    elf_list b = parsed_data.second;

    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());

    int64_t total = 0;
    for (auto it_a = a.cbegin(), it_b = b.cbegin(); it_a != a.cend() && it_b != b.cend(); it_a++, it_b++) {
        total += std::abs(*it_b - *it_a);
    }

    return total;
}

int64_t part2(std::string data) {
    elf_plan parsed_data = parse_data(data);
    elf_list a = parsed_data.first;
    elf_list b = parsed_data.second;

    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());

    int64_t total = 0;
    for (auto va : a) {
        auto appearances = std::count_if(b.begin(), b.end(), [va](int vb) {return vb == va; });
        total += va * appearances;
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
        std::cout << "Advent of Code 01" << std::endl;
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