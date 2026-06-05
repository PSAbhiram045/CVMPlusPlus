# 1. Set the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# 2. Name your project
project(CVM_Compiler)

# 3. Require C++17 (CRITICAL: std::variant requires C++17!)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 4. Tell CMake where to find your header (.h) files
include_directories(include)

# 5. Tell CMake to gather all your .cpp files
set(SOURCES
    src/main.cpp
    src/Lexer.cpp
    src/Parser.cpp
    src/Compiler.cpp
    src/VM.cpp
)

# 6. Build the final executable named "cvm" using those sources
add_executable(cvm ${SOURCES})