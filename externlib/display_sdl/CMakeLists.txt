cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set(NAME display_sdl)
project(${NAME})

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 14)


#list(APPEND CMAKE_MODULE_PATH "$ENV{HOME}/.brew/share/SFML/cmake/Modules/")

#find_package(SFML QUIET COMPONENTS system window graphics network audio)
#if (NOT SFML_FOUND)
#    include_directories(${PROJECT_BINARY_DIR}/../sfml/src/sfml/include)
#    link_directories(${PROJECT_BINARY_DIR}/../sfml/src/sfml-build/lib)
#endif()

link_directories(/usr/lib/)
link_directories($ENV{HOME}/.brew/lib/)

add_library(
        ${NAME} SHARED

        src/DisplaySdl.cpp
        include/DisplaySdl.hpp
)

target_link_libraries(${NAME} PRIVATE
        SDL2 SDL2_image
        )

target_include_directories(
        ${NAME} PUBLIC
        $ENV{HOME}/.brew/include
        ${PROJECT_SOURCE_DIR}/../../incs/
        ${PROJECT_SOURCE_DIR}/include
)

