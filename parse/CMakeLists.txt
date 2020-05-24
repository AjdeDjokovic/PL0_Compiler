cmake_minimum_required(VERSION 3.14)
project(parse)

set(CMAKE_CXX_STANDARD 14)

add_executable(parse main.cpp parse.cpp parse.h data.h lex.cpp lex.h)