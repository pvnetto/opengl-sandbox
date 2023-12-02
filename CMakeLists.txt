cmake_minimum_required(VERSION 3.10)

project("OpenGL Sandbox"
    VERSION 1.0
    DESCRIPTION "OpenGL sandbox project for study purposes."
    LANGUAGES CXX)

# Specifies the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${PROJECT_SOURCE_DIR}/src -I${PROJECT_SOURCE_DIR}/src/shared/layers")

# Disables building libraries as shared (dynamic) by default, and will default to static instead.
# We're doing this because we don't want to deal with copying DLLs over to the executable folder.
# https://discourse.cmake.org/t/why-is-manual-copying-of-dll-needed-when-linking-library/6971
# https://stackoverflow.com/questions/61386679/how-to-get-static-library-default-by-cmake
set(BUILD_SHARED_LIBS FALSE) 

add_subdirectory(src)
add_subdirectory(vendor)
add_subdirectory(modules)
