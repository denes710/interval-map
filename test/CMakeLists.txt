cmake_minimum_required(VERSION 3.22)
project(IntervalMapTestProject)

set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(IntervalMapTest test.cpp)
target_link_libraries(IntervalMapTest ${Boost_LIBRARIES} IntervalMapLib)
