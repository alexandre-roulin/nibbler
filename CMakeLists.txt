cmake_minimum_required(VERSION 3.12)
project(nibbler)

set(CMAKE_CXX_STANDARD 17)

include_directories(class incs)

add_executable(nibbler class/main.cpp class/components/ComponentMask.cpp class/components/ComponentMask.hpp class/components/MotionComponent.hpp class/ECS.cpp class/ECS.hpp incs/nibbler.hpp class/managers/SystemManager.cpp class/managers/SystemManager.hpp class/components/Component.hpp incs/nibbler.hpp class/managers/Pool.hpp class/managers/IPool.hpp class/managers/ComponentManager.cpp class/managers/ComponentManager.hpp class/entity/Entity.hpp class/managers/EntitiesManager.cpp class/managers/EntitiesManager.hpp class/systems/MotionSystem.cpp class/systems/MotionSystem.hpp class/components/TransformComponent.hpp class/systems/ASystem.hpp class/factories/EntityFactory.cpp class/factories/EntityFactory.hpp class/systems/TransformSystem.cpp class/systems/TransformSystem.hpp class/factories/SystemFactory.cpp class/factories/SystemFactory.hpp)