cmake_minimum_required(VERSION 3.6)
project(c_sniffer_1)

set(CMAKE_C_STANDARD 11)

include(FindPCAP.cmake)

set(SOURCE_FILES main.c)

add_executable(c_sniffer_1 ${SOURCE_FILES})
target_link_libraries(c_sniffer_1 ${PCAP_LIBRARY})