cmake_minimum_required(VERSION 3.10)
project(SA-Phrase-Search VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add example executable
add_executable(demo examples/demo.cpp)

# Installation
install(DIRECTORY include/ DESTINATION include/sa_phrase_search)

# Enable testing
enable_testing()

# Optional: Add test executable if tests directory exists
if(EXISTS ${PROJECT_SOURCE_DIR}/tests)
    add_subdirectory(tests)
endif()
