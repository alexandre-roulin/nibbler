
#include <iostream>
#include <KNU/managers/EntitiesManager.hpp>
#include <component/MotionComponent.hpp>
#include <component/TransformComponent.hpp>
#include <component/SpriteComponent.hpp>

using namespace KNU;


int main() {
	{

		auto entitiesManager = KNU::EntitiesManager();

		auto &entity = entitiesManager.createEntity();
		entitiesManager
				.addComponent<MotionComponent>(entity, RIGHT, 1)
				.addComponent<TransformComponent>(entity, 42, 42)
				.addComponent<SpriteComponent>(entity, "/path/to/sprite");

		std::cout << entity << std::endl;
		std::cout << entitiesManager.hasComponent<MotionComponent>(entity) << std::endl;
		std::cout << entitiesManager.hasComponent<TransformComponent>(entity) << std::endl;
		std::cout << entitiesManager.hasComponent<SpriteComponent>(entity) << std::endl;

		entitiesManager.removeComponent<MotionComponent>(entity);

		std::cout << entity << std::endl;
		std::cout << entitiesManager.hasComponent<MotionComponent>(entity) << std::endl;
	}

}












































