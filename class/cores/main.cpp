
#include <iostream>
#include <KNU/entities/Entity.hpp>
#include <KNU/World.hpp>
#include <component/TransformComponent.hpp>
#include <component/MotionComponent.hpp>
#include <component/SpriteComponent.hpp>
#include <systems/MotionSystem.hpp>


#include <logger.h>

int main() {
	char path[] = "/tmp/log.out";
	logger_init(path);
	{
		KNU::World world;

		auto &entity = world.createEntity();

		entity.addComponent<TransformComponent>(42, 42);
		entity.addComponent<MotionComponent>(NORTH, 1);
		entity.addComponent<SpriteComponent>("/vers/l'infini/et/l'au/delà");
		world.getSystemManager().addSystem<MotionSystem>();
		for (int index =0 ; index < 10 ; ++index) {
			world.update();
			world.getSystemManager().getSystem<MotionSystem>()->update();
			auto &position = entity.getComponent<TransformComponent>();
			log_debug("x : %d y : %d", position.x, position.y);
			std::cout << position << std::endl;
		}
		std::cout << entity.getComponent<TransformComponent>() << std::endl;
	}


}












































