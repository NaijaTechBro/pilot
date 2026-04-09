cmake_minimum_required(VERSION 4.1)
project(pilot)

set(CMAKE_CXX_STANDARD 20)

add_executable(pilot pilot.cpp
        headers/lexer.hpp)
