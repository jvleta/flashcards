# Minimum CMake version
cmake_minimum_required(VERSION 3.16)

# Project Name and Language
project(CardApp LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find and link nlohmann/json
find_package(nlohmann_json REQUIRED)

# Find and link GoogleTest
find_package(GTest REQUIRED)

# Add CLI program
add_executable(cli_program main.cpp)
target_link_libraries(cli_program PRIVATE nlohmann_json::nlohmann_json)

# Add test program
add_executable(test_program test.cpp)
target_link_libraries(test_program PRIVATE nlohmann_json::nlohmann_json GTest::GTest GTest::Main pthread)

# Enable testing
enable_testing()
add_test(NAME CardTests COMMAND test_program)
