cmake_minimum_required(VERSION 3.10)
project(CompilerDesignLabs C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Lexical Analyzer (CLI)
add_executable(lexer lexer.c)

# 2. Buffer Pairs Input Scheme Simulation (CLI)
add_executable(buffer_pairs buffer_pairs.cpp)

# 3. Left Recursion Elimination (CLI)
add_executable(left_recursion left_recursion.cpp)

# 4. Pure C++ Native Windows GUI Application
if(WIN32)
    add_executable(gui WIN32 gui.cpp)
    target_link_libraries(gui user32 gdi32 comctl32)
endif()
