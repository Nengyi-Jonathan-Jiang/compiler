cmake_minimum_required(VERSION 3.24)
project(compiler)

set(CMAKE_CXX_STANDARD 23)

add_executable(compiler main.cpp vm.h assembler.h)
