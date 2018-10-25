
#include <iostream>
#include <KNU/World.hpp>
#include <KNU/entities/Entity.hpp>
#include <component/TransformComponent.hpp>


int main() {

	{
		KNU::World world;

		KNU::Entity entity = world.createEntity();
		entity.getMask();
		std::cout << entity.hasComponent<TransformComponent>() << std::endl;
	}


}












































