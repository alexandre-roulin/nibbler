cmake_minimum_required(VERSION 3.12)

set(NAME display_sdl)
project(${NAME})

add_library(${NAME} MODULE
        src/DisplaySdl.cpp
        include/DisplaySdl.hpp)

target_link_libraries(${NAME} PRIVATE ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES})

target_include_directories(${NAME} PUBLIC ${SDL2_INCLUDE_DIR})
target_include_directories(${NAME} PUBLIC ${SDL2_IMAGE_INCLUDE_DIR})
target_include_directories(${NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

set_target_properties(${NAME}
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dynamicLibraries")
