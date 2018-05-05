cmake_minimum_required(VERSION 3.10)
project(Tensorflow-Cpp-API-Training)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_custom_target(OUTPUT main_1.elf COMMAND make run)