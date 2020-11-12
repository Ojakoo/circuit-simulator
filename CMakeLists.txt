cmake_minimum_required(VERSION 3.12)
project(circuit-sim-2020-2)

enable_testing()

# Bring headers into the project
include_directories(include)

# Bring Eigen into the project
include_directories(lib/eigen)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(dependencies)
add_subdirectory(src)
add_subdirectory(tests)
