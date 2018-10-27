#include <utility>


#include <iostream>
#include <KNU/entities/Entity.hpp>
#include <KNU/World.hpp>
#include <component/PositionComponent.hpp>
#include <component/MotionComponent.hpp>
#include <component/SpriteComponent.hpp>
#include <systems/MotionSystem.hpp>


#include <logger.h>
#include <component/JoystickComponent.hpp>
#include <events/JoystickEvent.hpp>
#include <systems/JoystickSystem.hpp>
#include <random>
#include <component/FollowComponent.hpp>
#include <systems/FollowSystem.hpp>
#include <component/CollectComponent.hpp>
#include <systems/CollisionSystem.hpp>
#include <systems/FoodSystem.hpp>

void init(KNU::World &world) {
	auto &snake_tail = world.createEntity();
	snake_tail.addComponent<PositionComponent>(7, 10);
	snake_tail.group(GROUP_SNAKE);
	snake_tail.tag(TAG_TAIL_SNAKE);

	auto snake_follow = snake_tail;
	for (int index = 0; index < 2; ++index) {
		auto &snake_body = world.createEntity();
		snake_body.addComponent<PositionComponent>(8 + index, 10);
		snake_body.group(GROUP_SNAKE);
		snake_follow.addComponent<FollowComponent>(snake_body.getId());
		snake_follow = snake_body;
	}
	auto &snake_head = world.createEntity();
	snake_head.addComponent<PositionComponent>(10, 10);
	snake_head.addComponent<JoystickComponent>();
	snake_head.addComponent<MotionComponent>();
	snake_head.addComponent<CollectComponent>();
	snake_head.group(GROUP_SNAKE);
	snake_head.tag(TAG_HEAD_SNAKE);
	snake_follow.addComponent<FollowComponent>(snake_head.getId());
}

void display(KNU::World &world) {
	for (const auto &entity : world.getEntityManager().getEntitiesByGroup(
			GROUP_SNAKE)) {
		auto positionComponent = entity.getComponent<PositionComponent>();
		log_info("ID [%d] Position [y:%d|x:%d]", entity.getId(),
				 positionComponent.y, positionComponent.x);
	}
}

int main() {
	char path[] = "/tmp/log.out";
	logger_init(path);
	KNU::World world;
	init(world);
	world.getSystemManager().addSystem<MotionSystem>();
	world.getSystemManager().addSystem<JoystickSystem>();
	world.getSystemManager().addSystem<FollowSystem>();
	world.getSystemManager().addSystem<CollisionSystem>();
	world.getSystemManager().addSystem<FoodSystem>();
	auto &food = world.createEntity();
	food.addComponent<PositionComponent>(11, 10);
	food.addComponent<CollectComponent>();
	food.tag("food");
	display(world);
	for (int index = 0; index < 2; ++index) {
		world.update();
		world.getEventManager().emitEvent<JoystickEvent>(ARROW_DOWN);
		world.getSystemManager().getSystem<FollowSystem>()->update();
		world.getSystemManager().getSystem<JoystickSystem>()->update();
		world.getSystemManager().getSystem<MotionSystem>()->update();
		world.getSystemManager().getSystem<CollisionSystem>()->update();
		world.getSystemManager().getSystem<FoodSystem>()->update();
		display(world);
	}
	for (int index = 0; index < 2; ++index) {
		world.update();
		world.getEventManager().emitEvent<JoystickEvent>(ARROW_RIGHT);
		world.getSystemManager().getSystem<FollowSystem>()->update();
		world.getSystemManager().getSystem<JoystickSystem>()->update();
		world.getSystemManager().getSystem<MotionSystem>()->update();
		world.getSystemManager().getSystem<CollisionSystem>()->update();
		world.getSystemManager().getSystem<FoodSystem>()->update();
		display(world);
	}
}