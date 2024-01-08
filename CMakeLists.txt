cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)

project(
    IntervalMapLibProject
    VERSION 1.0
    LANGUAGES CXX)

add_library(IntervalMapLib interval_map.hpp)
set_target_properties(IntervalMapLib PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(test)
