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

add_subdirectory(cmake_modules/boost-cmake)

link_directories(/usr/lib/)
link_directories($ENV{HOME}/.brew/lib/)
link_directories(${PROJECT_SOURCE_DIR}/imgui/)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

if (WIN32)
    find_package(OpenGL 3.3 REQUIRED)
elseif (UNIX AND NOT APPLE)
elseif (APPLE)
    include_directories(/System/Library/Frameworks)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    find_library(IOKit_LIBRARY IOKit)
    find_library(CoreVideo_LIBRARY CoreVideo)
    mark_as_advanced(COCOA_LIBRARY OpenGL_LIBRARY)
    set(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
    set(OPENGL_LIBRARIES ${LIBS} ${APPLE_LIBS})
else ()
    find_package(OpenGL 3.3 REQUIRED)
endif (WIN32)


set(PREFIX_SFML "${CMAKE_SOURCE_DIR}/cmake_modules/sfml")
set(SFML_PATH ${PREFIX_SFML})
find_package(SFML QUIET COMPONENTS system window graphics audio)
if (NOT SFML_FOUND OR NOT SFML_INCLUDE_DIR_FOUND)
    message(STATUS "SFML not found - will build from source")

    ExternalProject_Add(SFML
            PREFIX ${PREFIX_SFML}
            GIT_REPOSITORY https://github.com/SFML/SFML.git

            UPDATE_COMMAND ""

            INSTALL_DIR ${SFML_PATH}
            INSTALL_COMMAND make -j

            CMAKE_ARGS
            "-DCMAKE_INSTALL_PREFIX=${SFML_PATH}"
            "-DCMAKE_INSTALL_FRAMEWORK_PREFIX=${SFML_PATH}"
            "-DSFML_MISC_INSTALL_PREFIX=${SFML_PATH}"

            CMAKE_CACHE_ARGS
            "-DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}"
            "-DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}"

            LOG_DOWNLOAD 1 LOG_UPDATE 1 LOG_CONFIGURE 1 LOG_BUILD 1 LOG_INSTALL 1)

    ExternalProject_Get_Property(SFML INSTALL_DIR)
    set(SFML_INCLUDE_DIR ${INSTALL_DIR}/src/SFML/include)
    list(APPEND SFML_LIBRARIES sfml-graphics sfml-window sfml-system sfml-audio)
    list(APPEND SFML_LIBRARIES_DIR ${INSTALL_DIR}/src/SFML-build/lib)

    link_directories(${SFML_LIBRARIES_DIR})
endif ()
include_directories(${SFML_INCLUDE_DIR})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SDL2 REQUIRED sdl2 sdl2_mixer sdl2_image)
message(" SDL2_INCLUDE_DIR: ${SDL2_INCLUDE_DIR}")
message(" SDL2_LIBRARIES: ${SDL2_LIBRARIES}")
message(" SDL2_MIXER_INCLUDE_DIR: ${SDL2_MIXER_INCLUDE_DIR}")

#find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
#find_package(SDL2_mixer REQUIRED)
#include_directories(${SDL2_MIXER_INCLUDE_DIR})
#find_package(SDL2_image REQUIRED)
#include_directories(${SDL2_IMAGE_INCLUDE_DIR})


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
        class/events/NextFrame.cpp
        class/events/NextFrame.hpp
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
        class/Univers.cpp
        class/Univers.hpp
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
        class/systems/FoodEatSystem.cpp
        class/systems/FoodEatSystem.hpp
        class/systems/CollisionSystem.cpp
        class/systems/CollisionSystem.hpp
        class/systems/FollowSystem.cpp
        class/systems/FollowSystem.hpp
        class/systems/FoodCreationSystem.cpp
        class/systems/FoodCreationSystem.hpp
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
        class/systems/SpriteSystem.cpp class/systems/SpriteSystem.hpp class/gui/widget/WidgetConnect.cpp class/gui/widget/WidgetConnect.hpp class/KINU/Component.cpp class/KINU/Component.hpp class/KINU/Entity.cpp class/KINU/Entity.hpp class/KINU/EventsManager.cpp class/KINU/EventsManager.hpp class/KINU/Pool.hpp class/KINU/SystemsManager.cpp class/KINU/SystemsManager.hpp class/KINU/World.cpp class/KINU/World.hpp class/KINU/Config.hpp)

if (NOT SFML_FOUND OR NOT SFML_INCLUDE_DIR_FOUND)
    add_dependencies(${NAME} SFML)
endif ()

message("SFML_LIBRARIES ${SFML_LIBRARIES}")
message("SFML_INCLUDE_DIR ${SFML_INCLUDE_DIR}")

include_directories(${SFML_INCLUDE_DIR})


add_subdirectory(imgui)
add_subdirectory(extern/display_sdl)
add_subdirectory(extern/display_sfml)
add_subdirectory(extern/sound_sdl)
add_subdirectory(extern/sound_sfml)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIMGUI_USER_CONFIG=\"<../imconfig-sfml.h>\" -fsanitize=address -g3")

target_include_directories(
        ${NAME} PUBLIC
        class
        incs
        $ENV{HOME}/.brew/include
        ${PROJECT_SOURCE_DIR}/imgui/
        ${PROJECT_SOURCE_DIR}/imgui/imgui/
        ${PROJECT_SOURCE_DIR}/imgui/imgui-sfml/
        ${PROJECT_SOURCE_DIR}/extern/commun/
)


target_link_libraries(${NAME} PRIVATE Boost::filesystem)
target_link_libraries(${NAME} PRIVATE Boost::thread)
target_link_libraries(${NAME} PRIVATE Boost::program_options)
#target_link_libraries(${NAME} PRIVATE ${SDL2_LIBRARY})
#target_link_libraries(${NAME} PRIVATE ${SDL2_MIXER_LIBRARY})
target_link_libraries(${NAME} PRIVATE ${OPENGL_LIBRARIES})

target_link_libraries(${NAME} PRIVATE
        ${SFML_LIBRARIES}
        imgui-sfml
        )
