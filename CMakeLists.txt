cmake_minimum_required(VERSION 3.12)
project(nibbler)

set(CMAKE_CXX_STANDARD 14)

include_directories(class)
include_directories(incs)

add_executable(nibbler
        class/component/MotionComponent.cpp
        class/component/MotionComponent.hpp
        class/component/SpriteComponent.hpp
        class/component/PositionComponent.hpp
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
        class/systems/MotionSystem.cpp
        class/systems/MotionSystem.hpp
        incs/logger.h
        incs/logger_utils.h
        incs/nibbler.hpp class/component/SpriteComponent.cpp class/component/PositionComponent.cpp class/component/FollowComponent.cpp class/component/FollowComponent.hpp class/component/JoystickComponent.cpp class/component/JoystickComponent.hpp class/systems/JoystickSystem.cpp class/systems/JoystickSystem.hpp class/KNU/events/EventManager.cpp class/KNU/events/EventManager.hpp class/events/JoystickEvent.cpp class/events/JoystickEvent.hpp class/KNU/managers/Pool_.hpp class/systems/FollowSystem.cpp class/systems/FollowSystem.hpp class/component/CollisionComponent.cpp class/component/CollisionComponent.hpp class/systems/CollisionSystem.cpp class/systems/CollisionSystem.hpp class/events/FoodEvent.cpp class/events/FoodEvent.hpp class/systems/FoodSystem.cpp class/systems/FoodSystem.hpp class/component/WallComponent.cpp class/component/WallComponent.hpp)
