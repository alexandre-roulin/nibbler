
#include <iostream>
#include <KNU/World.hpp>
#include <KNU/entities/Entity.hpp>
#include <component/TransformComponent.hpp>


int main() {

	{
		KNU::World world;

		KNU::Entity entity = world.createEntity();
		entity.getMask();
		entity.addComponent(TransformComponent(42, 42));
	}


}












































