cmake_minimum_required(VERSION 3.12)

#set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set(NAME display_minilibx)
project(${NAME})

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 14)

#link_directories(/usr/lib/)
#link_directories($ENV{HOME}/.brew/lib/)
#link_directories(/usr/local/lib/)

#find_library(MINILIBX_LIBRARY mlx HINTS /usr/local/lib/)
find_library(MINILIBX_LIBRARY mlx HINTS minilibx)
message("MINILIBX_LIBRARY: ${MINILIBX_LIBRARY}")

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
endif ()

#add_library(
#        ${NAME} MODULE
#        src/DisplayMinilibx.cpp
#        include/DisplayMinilibx.hpp
#)
add_executable(${NAME}
        src/DisplayMinilibx.cpp
        include/DisplayMinilibx.hpp)



target_include_directories(
        ${NAME} PUBLIC
        /usr/local/include/
        $ENV{HOME}/.brew/include
        ${PROJECT_SOURCE_DIR}/../../incs/
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/minilibx
        minilibx/
)

target_link_libraries(${NAME} PUBLIC
        		${MINILIBX_LIBRARY}
				${OPENGL_LIBRARIES})
message("OPENGL_LIBRARIES: ${OPENGL_LIBRARIES}")


#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g3 -framework OpenGL -framework AppKit -framework Opencl -L/usr/local/lib/ -I/usr/local/include -lmlx -framework OpenGL -framework AppKit")
