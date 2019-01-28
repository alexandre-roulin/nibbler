cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set(NAME imgui-sfml)
project(${NAME})

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 14)


find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
        # Update submodules as needed
        option(GIT_SUBMODULE "Check submodules during build" ON)
        if(GIT_SUBMODULE)
                message(STATUS "Submodule update")
                execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
                if(NOT GIT_SUBMOD_RESULT EQUAL "0")
                        message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
                endif()
        endif()
endif()


file(READ imgui-sfml/imconfig-SFML.h IMGUI-SFML_CONFIG)
file(READ imgui/imconfig.h IMGUI_CONFIG)

FILE(WRITE ${PROJECT_SOURCE_DIR}/imconfig-sfml.h
        "${IMGUI_CONFIG}"
        "${IMGUI-SFML_CONFIG}")


include_directories(${SFML_INCLUDE_DIR})
message("SFML_INCLUDE_DIR : ${SFML_INCLUDE_DIR}")
#include_directories(${PROJECT_BINARY_DIR}/../sfml/src/sfml/include)


add_library(
        ${NAME} STATIC
        imconfig-sfml.h

        imgui-sfml/imgui-SFML.h
        imgui-sfml/imgui-SFML.cpp

        imgui/imgui.cpp
        imgui/imgui.h
        imgui/imgui_draw.cpp
        imgui/imgui_internal.h
        imgui/imgui_widgets.cpp
        imgui/imstb_rectpack.h
        imgui/imstb_textedit.h
        imgui/imstb_truetype.h
        )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIMGUI_USER_CONFIG=\"<../imconfig-sfml.h>\" -fsanitize=address -g3")

target_include_directories(
        ${NAME} PUBLIC
        $ENV{HOME}/.brew/include
        ${PROJECT_SOURCE_DIR}/imgui/
        ${PROJECT_SOURCE_DIR}/imgui-sfml/
)

