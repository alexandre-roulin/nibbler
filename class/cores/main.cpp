
#include <iostream>
#include <KNU/entities/Entity.hpp>
#include <KNU/World.hpp>
#include <component/TransformComponent.hpp>
#include <component/MotionComponent.hpp>
#include <component/SpriteComponent.hpp>
#include <systems/MotionSystem.hpp>


#include <logger.h>

void init(KNU::World &world, std::string s) {
	auto &entity = world.createEntity();
	entity.group("snake");
	entity.tag(s);
	entity.addComponent<TransformComponent>(42, 42);
	entity.addComponent<MotionComponent>(NORTH, 1);
	entity.addComponent<SpriteComponent>("/vers/l'infini/et/l'au/delà");
}

int main() {
	char path[] = "/tmp/log.out";
	logger_init(path);
	{
		KNU::World world;

		init(world, "snake1");
		init(world, "snake2");
		world.getSystemManager().addSystem<MotionSystem>();


		for (int index = 0; index < 2; ++index) {
			world.update();
			world.getSystemManager().getSystem<MotionSystem>()->update();
			auto position = world.getEntityManager().getEntitiesByGroup("snake");
			for (const auto &item : position) {
				auto &pos = item.getComponent<TransformComponent>();
				log_debug("ID: %d x : %d y : %d", item.getId(), pos.x, pos.y);
			}
		}

		auto entitySnake1 = world.getEntityManager().getEntityByTag("snake1");

		entitySnake1.kill();

		for (int index = 0; index < 2; ++index) {
			world.update();
			world.getSystemManager().getSystem<MotionSystem>()->update();
			auto position = world.getEntityManager().getEntitiesByGroup(
					"snake");
			for (const auto &item : position) {
				auto &pos = item.getComponent<TransformComponent>();
				log_debug("Tag : %d x : %d y : %d", item.getId(), pos.x, pos.y);
			}
		}

		init(world, "snake3");

		for (int index = 0; index < 2; ++index) {
			world.update();
			world.getSystemManager().getSystem<MotionSystem>()->update();
			auto position = world.getEntityManager().getEntitiesByGroup(
					"snake");
			for (const auto &item : position) {
				auto &pos = item.getComponent<TransformComponent>();
				log_debug("Tag : %d x : %d y : %d", item.getId(), pos.x, pos.y);
			}
		}
	}


}












































