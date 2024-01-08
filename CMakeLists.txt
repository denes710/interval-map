cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)

project(
    IntervalMapLibProject
    VERSION 1.0
    LANGUAGES CXX)

add_library(IntervalMapLib INTERFACE)
target_include_directories(IntervalMapLib INTERFACE include/)

add_subdirectory(test)
