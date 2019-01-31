cmake_minimum_required(VERSION 3.12)

set(NAME sound_sfml)
project(${NAME})

add_library(${NAME} MODULE
        src/SoundSfml.cpp
        include/SoundSfml.hpp)

target_link_libraries(${NAME} PRIVATE
        sfml-system sfml-audio)

target_include_directories(${NAME} PUBLIC ${SFML_INCLUDE_DIR})
target_include_directories(${NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

set_target_properties(${NAME}
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dynamicLibraries")