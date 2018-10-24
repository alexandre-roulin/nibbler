cmake_minimum_required(VERSION 3.12)
project(nibbler)

set(CMAKE_CXX_STANDARD 14)

include_directories(class)
include_directories(incs)

add_executable(nibbler
        Makefile class/KNU/utils/Signature.cpp class/KNU/utils/Signature.hpp class/cores/main.cpp class/KNU/component/Component.cpp class/KNU/component/Component.hpp class/KNU/component/TransformComponent.cpp class/KNU/component/TransformComponent.hpp class/KNU/managers/ComponentManager.cpp class/KNU/managers/ComponentManager.hpp class/KNU/managers/Pool.cpp class/KNU/managers/Pool.hpp class/KNU/entities/Entity.hpp class/KNU/managers/IPool.hpp)
