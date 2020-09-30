cmake_minimum_required(VERSION 3.17)
project(cpp_compressor)

set(CMAKE_CXX_STANDARD 17)

add_executable(compress main.cpp ari.h ari.cpp utils.cpp utils.h)
