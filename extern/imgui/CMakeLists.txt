cmake_minimum_required(VERSION 3.10)

set(NAME imgui-sfml)
project(${NAME})

set(CMAKE_CXX_STANDARD 17)

file(READ imgui-sfml/imconfig-SFML.h IMGUI-SFML_CONFIG)
file(READ imgui/imconfig.h IMGUI_CONFIG)

FILE(WRITE ${PROJECT_SOURCE_DIR}/imconfig-sfml.h
        "${IMGUI_CONFIG}"
        "${IMGUI-SFML_CONFIG}")


include_directories(${SFML_INCLUDE_DIR})
message("SFML_INCLUDE_DIR : ${SFML_INCLUDE_DIR}")

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

target_include_directories(
        ${NAME} PUBLIC
        $ENV{HOME}/.brew/include
)

