cmake_minimum_required(VERSION 3.10)
project(huffman)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(huffman
        main.cpp
        huffman.cpp
        huffman.hpp
        )
