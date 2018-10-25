cmake_minimum_required(VERSION 3.12)
project(nibbler)

set(CMAKE_CXX_STANDARD 17)

include_directories(class)
include_directories(incs)

add_executable(nibbler
        class/component/MotionComponent.hpp Makefile class/KNU/utils/Signature.cpp class/KNU/utils/Signature.hpp class/cores/main.cpp class/KNU/component/Component.cpp class/KNU/component/Component.hpp class/component/TransformComponent.hpp class/KNU/managers/ComponentManager.cpp class/KNU/managers/ComponentManager.hpp class/KNU/managers/Pool.cpp class/KNU/managers/Pool.hpp class/KNU/entities/Entity.hpp class/KNU/managers/AbstractPool.hpp incs/nibbler.hpp class/component/SpriteComponent.hpp class/KNU/managers/EntitiesManager.cpp class/KNU/managers/EntitiesManager.hpp class/KNU/World.cpp class/KNU/World.hpp class/KNU/managers/SystemManager.cpp class/KNU/managers/SystemManager.hpp class/systems/MotionSystem.cpp class/systems/MotionSystem.hpp class/KNU/systems/System.hpp class/component/MotionComponent.cpp class/KNU/systems/System.cpp class/KNU/entities/Entity.cpp class/KNU/managers/AbstractPool.cpp)
