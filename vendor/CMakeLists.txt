add_subdirectory(glm)
add_subdirectory(stb)
add_subdirectory(freetype)

set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_WARNINGS_AS_ERRORS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING")
add_subdirectory(assimp)

# Builds imgui library
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(IMGUI_SOURCES imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp imgui/imgui_demo.cpp imgui/imgui_tables.cpp)
set(IMGUI_HEADERS imgui/imgui.h imgui/imgui_internal.h)

add_library(imgui STATIC ${IMGUI_HEADERS} ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/vendor/imgui"
                                         "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
