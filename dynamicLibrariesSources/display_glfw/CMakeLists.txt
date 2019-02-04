cmake_minimum_required(VERSION 3.0)
set(NAME display_glfw)
project(${NAME})

#add_definitions(-DGLFW_INCLUDE_NONE -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${NAME} src/main.cpp
#add_library(${NAME} MODULE
        ../../extern/glad/src/glad.c
       src/ActModel.cpp
       src/ActModel.hpp
       src/Camera.cpp
       src/Camera.hpp
       src/DisplayGlfw.cpp
       src/DisplayGlfw.hpp
       src/Glfw.cpp
       src/Glfw.hpp
       src/Light.cpp
       src/Light.hpp
       src/Material.cpp
       src/Material.hpp
       src/Mesh.cpp
       src/Mesh.hpp
       src/Model.cpp
       src/Model.hpp
       src/Particle.cpp
       src/Particle.hpp
       src/Shader.cpp
       src/Shader.hpp
       src/Skybox.cpp
       src/Skybox.hpp
        ../../commun/KeyState.cpp ../../commun/KeyState.hpp ../../commun/KeyStateManager.cpp ../../commun/KeyStateManager.hpp)

set_target_properties(${NAME}
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dynamicLibraries"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tmp")

target_link_libraries(${NAME} assimp glfw Boost::filesystem
        ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
