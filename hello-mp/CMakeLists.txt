cmake_minimum_required(VERSION 3.6)
project(hello_mp)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp)
add_executable(hello_mp ${SOURCE_FILES})

# for openmp
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-4.9")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fopenmp")