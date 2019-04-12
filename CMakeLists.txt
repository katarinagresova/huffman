cmake_minimum_required(VERSION 3.10)
project(huffman)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(huff_codec
        main.cpp
        huffman_static.cpp
        huffman_static.hpp
        huffman_adaptive.cpp
        huffman_adaptive.hpp file_manipulation.cpp file_manipulation.hpp)
