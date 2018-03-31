cmake_minimum_required(VERSION 3.6)
project(dlib_test)
add_subdirectory(dlib dlib_build)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(dlib_test ${SOURCE_FILES})