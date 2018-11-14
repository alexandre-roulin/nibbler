cmake_minimum_required(VERSION 3.10)

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
add_subdirectory(imgui)

execute_process(COMMAND cd ${PROJECT_SOURCE_DIR}/jsoncpp/ && cmake . && make
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/jsoncpp)

link_directories(/usr/lib/)
link_directories($ENV{HOME}/.brew/lib/)
link_directories(${PROJECT_SOURCE_DIR}/imgui/)
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
        class/KINU/Component.cpp
        class/KINU/Component.h
        class/KINU/Config.h
        class/KINU/Entity.cpp
        class/KINU/Entity.h
        class/KINU/Event.cpp
        class/KINU/Event.h
        class/KINU/Pool.h
        class/KINU/System.cpp
        class/KINU/System.h
        class/KINU/Univers.cpp
        class/KINU/Univers.hpp
        class/KINU/World.cpp
        class/KINU/World.h
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
        class/KINU/Univers.cpp
        class/KINU/Univers.hpp
        externlib/commun/IDisplay.hpp
        externlib/commun/ISound.hpp

        #imgui/imgui-sfml/imgui-SFML.h
        #imgui/imgui-sfml/imgui-SFML.cpp
#
        #imgui/imconfig-sfml.h
        #imgui/imgui/imgui.cpp
        #imgui/imgui/imgui.h
#
        #imgui/imgui/imgui_draw.cpp
        #imgui/imgui/imgui_internal.h
        #imgui/imgui/imgui_widgets.cpp
        #imgui/imgui/imstb_rectpack.h
        #imgui/imgui/imstb_textedit.h
        #imgui/imgui/imstb_truetype.h

        incs/IWidget.hpp
        incs/logger.h
        incs/logger_utils.h
        incs/nibbler.hpp
        incs/stb_image.h
        )

#target_compile_options(${NAME} PUBLIC -Wall -Wextra -Werror -Wvla)
#target_compile_options(${NAME} PUBLIC -fsanitize=address -g3 -DENABLE_ASAN=ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIMGUI_USER_CONFIG=\"<../imconfig-sfml.h>\" -fsanitize=address -g3")

target_include_directories(
        ${NAME} PUBLIC
        class
        incs
        jsoncpp/include
        #SFML/include
        $ENV{HOME}/.brew/include
        ${PROJECT_SOURCE_DIR}/imgui/
        ${PROJECT_SOURCE_DIR}/imgui/imgui/
        ${PROJECT_SOURCE_DIR}/imgui/imgui-sfml/
        ${PROJECT_SOURCE_DIR}/externlib/commun/
)


target_link_libraries(${NAME} PUBLIC
        sfml-graphics sfml-window sfml-system sfml-audio
        jsoncpp
        imgui-sfml
        boost_system boost_thread-mt
        ${LIBS}
        )
