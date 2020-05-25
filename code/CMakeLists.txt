cmake_minimum_required(VERSION 3.14)
project(test)

set(CMAKE_CXX_STANDARD 14)

add_executable(test main.cpp parse.cpp parse.h lex.cpp lex.h data.h)