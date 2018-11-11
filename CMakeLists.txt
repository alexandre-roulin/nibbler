cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set(NAME nibbler)
project(${NAME})

set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_MACOSX_RPATH 1)

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

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/SFML/CMakeLists.txt")
    message(FATAL_ERROR "The SFML submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

#add_subdirectory(SFML)

link_directories(/usr/lib/)
link_directories($ENV{HOME}/.brew/lib/)
#link_directories(${PROJECT_SOURCE_DIR}/SFML/)

if(WIN32)
elseif(UNIX AND NOT APPLE)
elseif(APPLE)
    include_directories(/System/Library/Frameworks)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    find_library(IOKit_LIBRARY IOKit)
    find_library(CoreVideo_LIBRARY CoreVideo)
    mark_as_advanced(COCOA_LIBRARY OpenGL_LIBRARY)
    set(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
    set(LIBS ${LIBS} ${APPLE_LIBS})
else()
    set(LIBS)
endif(WIN32)

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
        class/events/FoodEvent.cpp
        class/events/FoodEvent.hpp
        class/events/JoystickEvent.cpp
        class/events/JoystickEvent.hpp
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
        class/gui/Core.cpp
        class/gui/Core.hpp
        class/gui/imgui_demo.cpp
        class/KNU/component/Component.cpp
        class/KNU/component/Component.hpp
        class/KNU/entities/Entity.cpp
        class/KNU/entities/Entity.hpp
        class/KNU/events/EventManager.cpp
        class/KNU/events/EventManager.hpp
        class/KNU/managers/AbstractPool.hpp
        class/KNU/managers/Pool.hpp
        class/KNU/managers/Pool_.hpp
        class/KNU/systems/System.cpp
        class/KNU/systems/System.hpp
        class/KNU/utils/Signature.cpp
        class/KNU/utils/Signature.hpp
        class/KNU/World.cpp
        class/KNU/World.hpp
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
        class/network/chat_message.hpp
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
        externlib/commun/IDisplay.hpp
        externlib/commun/ISound.hpp
        #externlib/display_sdl/include/DisplaySdl.hpp
        #externlib/display_sdl/src/DisplaySdl.cpp
        #externlib/display_sfml/include/DisplaySfml.hpp
        #externlib/display_sfml/src/DisplaySfml.cpp
        #externlib/sandbox/main.cpp
        #externlib/sound_sdl/include/SoundSdl.hpp
        #externlib/sound_sdl/src/main.cpp
        #externlib/sound_sdl/src/SoundSdl.cpp
        #externlib/sound_sfml/include/SoundSfml.hpp
        #externlib/sound_sfml/src/main.cpp
        #externlib/sound_sfml/src/SoundSfml.cpp
        imgui-sfml/imconfig.h
        imgui-sfml/imgui-SFML.cpp
        imgui-sfml/imgui-SFML.h
        imgui-sfml/imgui.cpp
        imgui-sfml/imgui.h
        imgui-sfml/imgui_draw.cpp
        imgui-sfml/imgui_internal.h
        imgui-sfml/imgui_widgets.cpp
        imgui-sfml/imstb_rectpack.h
        imgui-sfml/imstb_textedit.h
        imgui-sfml/imstb_truetype.h
        incs/IWidget.hpp
        incs/logger.h
        incs/logger_utils.h
        incs/nibbler.hpp
        incs/stb_image.h
        )

#target_compile_options(${NAME} PUBLIC -Wall -Wextra -Werror -Wvla)
#target_compile_options(${NAME} PUBLIC -fsanitize=address -g3 -DENABLE_ASAN=ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g3")

target_include_directories(
        ${NAME} PUBLIC
        class
        incs
        jsoncpp/include
        #SFML/include
        $ENV{HOME}/.brew/include
        imgui-sfml/
        ${PROJECT_SOURCE_DIR}/externlib/commun/
)


target_link_libraries(${NAME} PUBLIC
        sfml-graphics sfml-window sfml-system sfml-audio
        jsoncpp
        boost_system boost_thread-mt
        ${LIBS}
        )
