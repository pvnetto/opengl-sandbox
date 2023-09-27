set(SPW_SOURCES SimpleWindow.cpp)

add_library(spw STATIC ${SPW_SOURCES})

target_link_libraries(spw glfw)

target_include_directories(spw PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
                                         "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")