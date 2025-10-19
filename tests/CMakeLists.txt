cmake_minimum_required(VERSION 3.10)

# Add test executable
add_executable(test_basic test_basic.cpp)

# Add test to CTest
add_test(NAME BasicTests COMMAND test_basic)
