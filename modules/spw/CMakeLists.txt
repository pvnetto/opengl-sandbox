cmake_minimum_required(VERSION 3.10)

project(SimpleWindow VERSION 1.0 LANGUAGES C)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

add_subdirectory(spw)
add_subdirectory(vendor)
