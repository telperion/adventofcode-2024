cmake_minimum_required(VERSION 3.7)
project(advent_of_code VERSION 2024.01.01)

include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE DEBUG)

add_executable(day_01 01/day_01.cpp)
add_test(NAME test_01_part1 COMMAND day_01 -1)
add_test(NAME test_01_part2 COMMAND day_01 -2)
