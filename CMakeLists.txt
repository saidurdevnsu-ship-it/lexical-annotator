cmake_minimum_required(VERSION 3.10)
project(CompilerDesignLabs C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Lexical Analyzer
add_executable(lexer 2.c)

# 2. Buffer Pairs Input Scheme Simulation
add_executable(buffer_pairs assignment1.cpp)

# 3. Left Recursion Elimination
add_executable(left_recursion saidur.cpp)
