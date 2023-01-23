cmake_minimum_required(VERSION 3.0)
project(cs4532_lab01)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(llist llist)
#include_directories(llist)

add_executable(cs4532_lab01 src/main.cpp src/generator.cpp)
target_link_libraries(cs4532_lab01 llist)

