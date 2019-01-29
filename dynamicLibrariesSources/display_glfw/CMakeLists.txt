cmake_minimum_required(VERSION 3.0)
set(NAME display_glfw)
project(${NAME})

include_directories(include/)

file(GLOB PROJECT_HEADERS src/*.hpp include/*.hpp)
file(GLOB PROJECT_SOURCES src/*.cpp)

source_group("Headers" FILES ${PROJECT_HEADERS})
#source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${EXTERN_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
#add_executable(${NAME}
#${PROJECT_SOURCES} ${PROJECT_HEADERS}
#${EXTERN_SOURCES_DISPLAY_GLFW})

add_library(${NAME} MODULE
${PROJECT_SOURCES} ${PROJECT_HEADERS}
${EXTERN_SOURCES_DISPLAY_GLFW})

set_target_properties(${NAME}
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dynamicLibraries"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tmp")

target_link_libraries(${NAME} assimp glfw Boost::filesystem
        ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
