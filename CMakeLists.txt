cmake_minimum_required(VERSION 3.12)

include(ExternalProject)
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set(NAME nibbler)
project(${NAME})


set(CMAKE_CXX_STANDARD 14)

find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if (GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if (NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif ()
    endif ()
endif ()

link_directories(/usr/lib/)
link_directories($ENV{HOME}/.brew/lib/)
link_directories(${PROJECT_SOURCE_DIR}/imgui/)


if (WIN32)
elseif (UNIX AND NOT APPLE)
elseif (APPLE)
    include_directories(/System/Library/Frameworks)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    find_library(IOKit_LIBRARY IOKit)
    find_library(CoreVideo_LIBRARY CoreVideo)
    mark_as_advanced(COCOA_LIBRARY OpenGL_LIBRARY)
    set(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
    set(LIBS ${LIBS} ${APPLE_LIBS})
else ()
    set(LIBS)
endif (WIN32)

list(APPEND CMAKE_MODULE_PATH "$ENV{HOME}/.brew/share/SFML/cmake/Modules/")

find_package(SFML QUIET COMPONENTS system window graphics network audio)
if (NOT SFML_FOUND)

    set(sfml_PREFIX "${PROJECT_SOURCE_DIR}/sfml")
    set(sfml_INSTALL_DIR "${PROJECT_SOURCE_DIR}/sfml")
    list(APPEND sfml_CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${sfml_INSTALL_DIR} -DCMAKE_INSTALL_FRAMEWORK_PREFIX=${sfml_INSTALL_DIR} -DSFML_MISC_INSTALL_PREFIX=${sfml_INSTALL_DIR}")

    ExternalProject_Add(sfml
            PREFIX ${sfml_PREFIX}
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            INSTALL_DIR ${sfml_INSTALL_DIR}
            CMAKE_ARGS "${sfml_CMAKE_ARGS}"
            INSTALL_COMMAND make
            )

    include_directories(${sfml_INSTALL_DIR}/src/sfml/include)
    link_directories(${sfml_INSTALL_DIR}/src/sfml-build/lib)

endif ()

UNSET(JSONCPP_LIBRARY CACHE)
find_library(JSONCPP_LIBRARY
        NAMES jsoncpp
        HINTS "$ENV{HOME}/.brew/lib/")
if (NOT JSONCPP_LIBRARY)
    ExternalProject_Add(jsoncpp
            PREFIX ${PROJECT_SOURCE_DIR}/jsoncpp
            GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp
            INSTALL_DIR {PROJECT_SOURCE_DIR}/jsoncpp
            CMAKE_ARGS ""
            INSTALL_COMMAND make
            )

    include_directories(${PROJECT_SOURCE_DIR}/jsoncpp/src/jsoncpp/include/)
    link_directories(${PROJECT_SOURCE_DIR}/jsoncpp/src/jsoncpp-build/src/lib_json/)
    link_libraries(${PROJECT_SOURCE_DIR}/jsoncpp/src/jsoncpp-build/src/lib_json/libjsoncpp.a)
else ()
    link_libraries(${JSONCPP_LIBRARY})
endif ()

add_executable(${NAME}
        class/component/CollisionComponent.cpp
        class/component/CollisionComponent.hpp
        class/component/FollowComponent.cpp
        class/component/FollowComponent.hpp
        class/component/JoystickComponent.cpp
        class/component/JoystickComponent.hpp
        class/component/MotionComponent.cpp
        class/component/MotionComponent.hpp
        class/component/PositionComponent.cpp
        class/component/PositionComponent.hpp
        class/component/SpriteComponent.cpp
        class/component/SpriteComponent.hpp
        class/cores/main.cpp
        class/events/StartEvent.cpp
        class/events/StartEvent.hpp
        class/events/FoodEat.cpp
        class/events/FoodEat.hpp
        class/events/JoystickEvent.cpp
        class/events/JoystickEvent.hpp
        class/events/FoodCreation.hpp
        class/events/FoodCreation.cpp
        class/factory/Factory.cpp
        class/factory/Factory.hpp
        class/gui/widget/AWidget.cpp
        class/gui/widget/AWidget.hpp
        class/gui/widget/WidgetChat.cpp
        class/gui/widget/WidgetChat.hpp
        class/gui/widget/WidgetExit.cpp
        class/gui/widget/WidgetExit.hpp
        class/gui/widget/WidgetLobby.cpp
        class/gui/widget/WidgetLobby.hpp
        class/gui/widget/WidgetSnake.cpp
        class/gui/widget/WidgetSnake.hpp
        class/gui/widget/WidgetOption.cpp
        class/gui/widget/WidgetOption.hpp
        class/gui/Core.cpp
        class/gui/Core.hpp
        class/gui/imgui_demo.cpp
        class/KINU/Component.cpp
        class/KINU/Component.hpp
        class/KINU/Config.hpp
        class/KINU/Entity.cpp
        class/KINU/Entity.hpp
        class/KINU/Event.cpp
        class/KINU/Event.hpp
        class/KINU/Pool.hpp
        class/KINU/System.cpp
        class/KINU/System.hpp
        class/Univers.cpp
        class/Univers.hpp
        class/KINU/World.cpp
        class/KINU/World.hpp
        class/logger/display/logger_debug.cpp
        class/logger/display/logger_error.cpp
        class/logger/display/logger_fatal.cpp
        class/logger/display/logger_info.cpp
        class/logger/display/logger_success.cpp
        class/logger/display/logger_trace.cpp
        class/logger/display/logger_warn.cpp
        class/logger/logger/logger_init.cpp
        class/logger/utils/logger_get_time.cpp
        class/logger/utils/logger_init_open_file.cpp
        class/network/ClientTCP.cpp
        class/network/ClientTCP.hpp
        class/network/Network.cpp
        class/network/Network.hpp
        class/network/ServerTCP.cpp
        class/network/ServerTCP.hpp
        class/network/UtilsTCP.cpp
        class/network/UtilsTCP.hpp
        class/systems/CollisionSystem.cpp
        class/systems/CollisionSystem.hpp
        class/systems/FollowSystem.cpp
        class/systems/FollowSystem.hpp
        class/systems/FoodSystem.cpp
        class/systems/FoodSystem.hpp
        class/systems/JoystickSystem.cpp
        class/systems/JoystickSystem.hpp
        class/systems/MotionSystem.cpp
        class/systems/MotionSystem.hpp
        class/systems/RenderSystem.cpp
        class/systems/RenderSystem.hpp
        class/Univers.cpp
        class/Univers.hpp

        incs/IDisplay.hpp
        incs/ISound.hpp
        incs/IWidget.hpp
        incs/logger.h
        incs/logger_utils.h
        incs/nibbler.hpp
        incs/stb_image.h
        class/systems/SpriteSystem.cpp class/systems/SpriteSystem.hpp)

if (NOT SFML_FOUND)
    add_dependencies(${NAME} sfml)
endif ()

if (NOT JSONCPP_LIBRARY)
    add_dependencies(${NAME} jsoncpp)
endif ()


add_subdirectory(imgui)
add_subdirectory(externlib/display_sdl)
add_subdirectory(externlib/display_sfml)
add_subdirectory(externlib/sound_sdl)
add_subdirectory(externlib/sound_sfml)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIMGUI_USER_CONFIG=\"<../imconfig-sfml.h>\" -fsanitize=address -g3")

target_include_directories(
        ${NAME} PUBLIC
        class
        incs
        #jsoncpp/include
        $ENV{HOME}/.brew/include
        #${PROJECT_SOURCE_DIR}/jsoncpp/include/
        ${PROJECT_SOURCE_DIR}/imgui/
        ${PROJECT_SOURCE_DIR}/imgui/imgui/
        ${PROJECT_SOURCE_DIR}/imgui/imgui-sfml/
        ${PROJECT_SOURCE_DIR}/externlib/commun/
)


target_link_libraries(${NAME} PRIVATE
        sfml-graphics sfml-window sfml-system sfml-audio
        imgui-sfml
        boost_system boost_thread-mt
        ${LIBS}
        )
