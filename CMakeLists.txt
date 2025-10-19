cmake_minimum_required(VERSION 3.10)
project(SA-Phrase-Search)

set(CMAKE_CXX_STANDARD 23)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(main main.cpp)