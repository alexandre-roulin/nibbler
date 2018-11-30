cmake_minimum_required(VERSION 3.12)

#set(CMAKE_C_COMPILER "/usr/bin/gcc")
#set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set(NAME display_minilibx)
project(${NAME})

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 14)

link_directories(/usr/lib/)
link_directories($ENV{HOME}/.brew/lib/)
link_directories(/usr/local/lib/)

find_library(MINILIBX_LIBRARY mlx HINTS /usr/local/lib/)

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

add_library(
        ${NAME} MODULE

        src/DisplayMinilibx.cpp
        include/DisplayMinilibx.hpp
)

target_link_libraries(${NAME} PRIVATE
        		${MINILIBX_LIBRARY}
				${LIBS}
        )


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g3 -framework OpenGL -framework AppKit -framework Opencl")


target_include_directories(
        ${NAME} PUBLIC
 		/usr/local/include/
        $ENV{HOME}/.brew/include
        ${PROJECT_SOURCE_DIR}/../../incs/
        ${PROJECT_SOURCE_DIR}/include
)

