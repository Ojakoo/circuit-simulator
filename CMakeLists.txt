cmake_minimum_required(VERSION 3.18.4)
project(circuit-sim-2020-2)

# Bring headers into the project
include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(main ${SOURCES})