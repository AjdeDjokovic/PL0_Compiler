cmake_minimum_required(VERSION 3.14)
project(compile)

set(CMAKE_CXX_STANDARD 14)

add_executable(compile main.cpp data.h lex.cpp lex.h)