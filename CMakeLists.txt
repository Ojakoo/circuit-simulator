cmake_minimum_required(VERSION 3.0)
project(circuit-sim-2020-2)

enable_testing()

# Bring headers into the project
include_directories(include)

# Doctest wants c++11
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src)
add_subdirectory(tests)