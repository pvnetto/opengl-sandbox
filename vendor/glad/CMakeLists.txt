# Copyright(c) 2019 spdlog authors Distributed under the MIT License (http://opensource.org/licenses/MIT)

cmake_minimum_required(VERSION 3.10)

project(Glad VERSION 1.0 LANGUAGES C)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

set(GLAD_SOURCES src/glad.c)
set(GLAD_HEADERS_DIR "${PROJECT_SOURCE_DIR}/include")
set(GLAD_ALL_HEADERS "${GLAD_HEADERS_DIR}/glad/glad.h" "${GLAD_HEADERS_DIR}/KHR/khrplatform.h")

add_library(glad STATIC ${GLAD_SOURCES} ${GLAD_ALL_HEADERS})

target_include_directories(glad PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
                                         "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
