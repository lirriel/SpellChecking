cmake_minimum_required(VERSION 3.7)
project(prj)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp dictionary.h hashset.cpp hashset.h dictionary.cpp)
add_executable(prj ${SOURCE_FILES})