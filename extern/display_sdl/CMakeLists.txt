cmake_minimum_required(VERSION 3.12)

set(NAME display_sdl)
project(${NAME})

add_library(${NAME} MODULE
        src/DisplaySdl.cpp
        include/DisplaySdl.hpp)

message("SDL2_INCLUDE_DIR:${SDL2_INCLUDE_DIR}")
message("SDL2_IMAGE_INCLUDE_DIR:${SDL2_IMAGE_INCLUDE_DIR}")
message("SDL2_LIBRARIES:${SDL2_LIBRARIES}")
message("SDL2_IMAGE_LIBRARIES:${SDL2_IMAGE_LIBRARIES}")

target_link_libraries(${NAME} PRIVATE
        ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES})

target_include_directories(${NAME} PUBLIC ${SDL2_INCLUDE_DIR})
target_include_directories(${NAME} PUBLIC ${SDL2_IMAGE_INCLUDE_DIR})
target_include_directories(${NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

