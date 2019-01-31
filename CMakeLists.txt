cmake_minimum_required(VERSION 3.10)

set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNIBBLER_ROOT_PROJECT_PATH=\\\"${CMAKE_SOURCE_DIR}\\\"")

if (WIN32)
    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
        if(NOT WIN32)
            set(GLAD_LIBRARIES dl)
        endif()
    endif()
    find_package(OpenGL 3.3 REQUIRED)
elseif (UNIX AND NOT APPLE)
elseif (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    if(INSTALL)
        execute_process(COMMAND $ENV{HOME}/.brew/bin/brew install sdl2)
        execute_process(COMMAND $ENV{HOME}/.brew/bin/brew install sdl2_image)
        execute_process(COMMAND $ENV{HOME}/.brew/bin/brew install sdl2_mixer)
        execute_process(COMMAND $ENV{HOME}/.brew/bin/brew install sfml)
        execute_process(COMMAND $ENV{HOME}/.brew/bin/brew install boost)
        execute_process(COMMAND $ENV{HOME}/.brew/bin/brew install assimp)
        execute_process(COMMAND $ENV{HOME}/.brew/bin/brew install glm)
    endif(INSTALL)

    link_directories(/usr/lib/)
    link_directories($ENV{HOME}/.brew/lib/)
    include_directories($ENV{HOME}/.brew/include)

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
endif ()
if(NOT WIN32)
    set(GLAD_LIBRARIES dl)
endif()

find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init extern/imgui extern/stb extern/glad cmake/sdl2-cmake-scripts
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if (NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif ()
endif ()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_SOURCE_DIR}/cmake/sdl2-cmake-scripts/;${CMAKE_MODULE_PATH}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIMGUI_USER_CONFIG=\"<../imconfig-sfml.h>\" -fsanitize=address -g3")
set(EXTERN_SOURCES_DISPLAY_GLFW "${CMAKE_SOURCE_DIR}/extern/glad/src/glad.c")


set(SDL2_PATH "$ENV{HOME}/.brew;D:\\lib\\SDL2-2.0.9\\")
find_package(SDL2 REQUIRED)
set(SDL2_IMAGE_PATH ${SDL2_SEARCH_PATHS})
set(SDL2_MIXER_PATH ${SDL2_SEARCH_PATHS})
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

find_package(SFML REQUIRED COMPONENTS audio graphics window system)
include_directories(${SFML_INCLUDE_DIR})

set(BOOST_ROOT $ENV{HOME}/.brew/)
find_package(Boost QUIET COMPONENTS program_options thread filesystem)
if (Boost_FOUND)
    message("EXTERN : Boost finded")
elseif (NOT TARGET Boost::boost)
    message("EXTERN : Unable to find Boost, cloning...")
    if (GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init cmake/boost-cmake
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if (NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif ()
    endif ()
    add_subdirectory(cmake/boost-cmake)
endif()


list(APPEND CMAKE_MODULE_PATH "~/.brew/lib/cmake/glm")
find_package(glm QUIET)
if (glm_FOUND)
    message("EXTERN : glm finded")
elseif (NOT TARGET glm)
    message("EXTERN : Unable to find glm, cloning...")
    if (GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init extern/glm
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if (NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif ()
    endif ()
    option(GLM_BUILD_DOCS OFF)
    option(GLM_BUILD_EXAMPLES OFF)
    option(GLM_TEST_ENABLE OFF)
    add_subdirectory(extern/glm)
endif()

list(APPEND CMAKE_MODULE_PATH "~/.brew/lib/cmake/glfw3")
find_package(glfw3 REQUIRED)

list(APPEND CMAKE_MODULE_PATH "~/.brew/lib/cmake/assimp")
find_package(assimp QUIET)
if (assimp_FOUND)
    message("EXTERN : assimp finded")
elseif (NOT TARGET assimp)
    message("EXTERN : Unable to find assimp, cloning...")
    if (GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init extern/assimp
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if (NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif ()
    endif ()
    option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
    option(ASSIMP_BUILD_SAMPLES OFF)
    option(ASSIMP_BUILD_TESTS OFF)
    add_subdirectory(extern/assimp)
endif()

include_directories(
        ${CMAKE_SOURCE_DIR}/nibblerSources/incs/
        ${CMAKE_SOURCE_DIR}/extern/imgui/
        ${CMAKE_SOURCE_DIR}/extern/imgui/imgui/
        ${CMAKE_SOURCE_DIR}/extern/imgui/imgui-sfml/
        ${CMAKE_SOURCE_DIR}/extern/glad/include/
        ${CMAKE_SOURCE_DIR}/extern/glfw/include/
        ${CMAKE_SOURCE_DIR}/extern/sfml/include/
        ${CMAKE_SOURCE_DIR}/extern/glm/
        ${CMAKE_SOURCE_DIR}/extern/stb/)

add_subdirectory(extern/imgui)

add_subdirectory(nibblerSources)
add_subdirectory(dynamicLibrariesSources/display_glfw)
add_subdirectory(dynamicLibrariesSources/display_sdl)
add_subdirectory(dynamicLibrariesSources/display_sfml)
add_subdirectory(dynamicLibrariesSources/sound_sdl)
add_subdirectory(dynamicLibrariesSources/sound_sfml)
