cmake_minimum_required(VERSION 3.12)
project(nibbler)

set(CMAKE_CXX_STANDARD 14)

include_directories(class)
include_directories(incs)
include_directories(logger/incs)

add_executable(nibbler
        class/component/MotionComponent.cpp
        class/component/MotionComponent.hpp
        class/component/SpriteComponent.hpp
        class/component/TransformComponent.hpp
        class/cores/main.cpp
        class/KNU/component/Component.cpp
        class/KNU/component/Component.hpp
        class/KNU/entities/Entity.cpp
        class/KNU/entities/Entity.hpp
        class/KNU/managers/AbstractPool.hpp
        class/KNU/managers/Pool.hpp
        class/KNU/systems/System.cpp
        class/KNU/systems/System.hpp
        class/KNU/utils/Signature.cpp
        class/KNU/utils/Signature.hpp
        class/KNU/World.cpp
        class/KNU/World.hpp
        class/systems/MotionSystem.cpp
        class/systems/MotionSystem.hpp
        incs/nibbler.hpp
        logger/incs/logger.h
        logger/incs/logger_utils.h
        logger/srcs/display/logger_debug.c
        logger/srcs/display/logger_error.c
        logger/srcs/display/logger_fatal.c
        logger/srcs/display/logger_info.c
        logger/srcs/display/logger_success.c
        logger/srcs/display/logger_trace.c
        logger/srcs/display/logger_warn.c
        logger/srcs/logger/logger_close.c
        logger/srcs/logger/logger_init.c
        logger/srcs/utils/logger_get_time.c
        logger/srcs/utils/logger_init_open_file.c
        logger/test/main.c)
