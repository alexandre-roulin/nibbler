
#include <iostream>
#include <KNU/World.hpp>
#include <KNU/entities/Entity.hpp>
#include <component/TransformComponent.hpp>
#include <component/MotionComponent.hpp>
#include <component/SpriteComponent.hpp>


int main() {

	{
		KNU::World world;

		KNU::Entity entity = world.createEntity();

		entity.addComponent<TransformComponent>(42, 42);
		entity.addComponent<MotionComponent>(NORTH, 1);
		entity.addComponent<SpriteComponent>("/vers/l'infini/et/l'au/delà");

		std::cout << entity.hasComponent<TransformComponent>() << std::endl;
	}


}












































