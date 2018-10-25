
#include <iostream>
#include <KNU/World.hpp>
#include <KNU/entities/Entity.hpp>

int main() {

	{
		KNU::World world;

		KNU::Entity entity = world.createEntity();
		entity.getMask();
		entity.hasGroup("");
	}


}












































