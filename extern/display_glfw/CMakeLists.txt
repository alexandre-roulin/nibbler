cmake_minimum_required(VERSION 3.0)
project(display_glfw)

set(CMAKE_CXX_STANDARD 17)

option(GLM_BUILD_DOCS OFF)
option(GLM_BUILD_EXAMPLES OFF)
option(GLM_TEST_ENABLE OFF)
add_subdirectory(extern/glm)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(extern/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(extern/assimp)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(
        include/
        ../../incs/
        extern/assimp/include/
        extern/glad/include/
        extern/glfw/include/
        extern/glm/
        extern/stb/)

file(GLOB EXTERN_SOURCES extern/glad/src/glad.c)
file(GLOB PROJECT_HEADERS src/*.hpp include/*.hpp)
file(GLOB PROJECT_SOURCES src/*.cpp)

file(GLOB PROJECT_CONFIGS CMakeLists.txt
        Readme.md
        .gitattributes
        .gitignore
        .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
#source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${EXTERN_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
        -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME}
${PROJECT_SOURCES} ${PROJECT_HEADERS}
#${PROJECT_SHADERS}
${PROJECT_CONFIGS}
${EXTERN_SOURCES})

#add_library(${PROJECT_NAME} MODULE
#${PROJECT_SOURCES} ${PROJECT_HEADERS}
##${PROJECT_SHADERS}
#${PROJECT_CONFIGS}
#${EXTERN_SOURCES})

target_link_libraries(${PROJECT_NAME} assimp glfw Boost::filesystem
        ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
