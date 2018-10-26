
#include <iostream>
#include <KNU/World.hpp>
#include <KNU/entities/Entity.hpp>
#include <component/TransformComponent.hpp>
#include <component/MotionComponent.hpp>
#include <component/SpriteComponent.hpp>
#include <systems/MotionSystem.hpp>


int main() {

	{
		KNU::World world;

		auto &entity = world.createEntity();

		entity.addComponent<TransformComponent>(42, 42);
		entity.addComponent<MotionComponent>(NORTH, 1);
		entity.addComponent<SpriteComponent>("/vers/l'infini/et/l'au/delà");

		world.getSystemManager().addSystem<MotionSystem>();
		std::cout << "Match System" << entity.getMask().matches(world.getSystemManager().getSystem<MotionSystem>()->getSignature()) << std::endl;
		std::cout << "Signature Entity : " << entity.getMask() << std::endl;
		std::cout << "System signature : " <<world.getSystemManager().getSystem<MotionSystem>()->getSignature() << std::endl;
		for (int index =0 ; index < 10 ; ++index) {
			world.update();
			world.getSystemManager().getSystem<MotionSystem>()->update();
			auto &position = entity.getComponent<TransformComponent>();
			std::cout << position << std::endl;
		}
		std::cout << entity.getComponent<TransformComponent>() << std::endl;
	}
 

}












































