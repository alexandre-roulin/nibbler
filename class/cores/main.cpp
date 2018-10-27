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

void init(KNU::World &world) {
	auto &snake_tail = world.createEntity();
	snake_tail.addComponent<PositionComponent>(7, 10);
	snake_tail.group(GROUP_SNAKE);
	snake_tail.tag(TAG_TAIL_SNAKE);

	auto snake_follow = snake_tail;
	for (int index = 0; index < 4; ++index) {
		auto &snake_body = world.createEntity();
		snake_body.addComponent<PositionComponent>(8 + index, 10);
		snake_body.group(GROUP_SNAKE);
		snake_follow.addComponent<FollowComponent>(snake_body.getId());
		snake_follow = snake_body;
	}
	auto &snake_head = world.createEntity();
	snake_head.addComponent<PositionComponent>(12, 10);
	snake_head.addComponent<JoystickComponent>();
	snake_head.addComponent<MotionComponent>();
	snake_head.group(GROUP_SNAKE);
	snake_head.tag(TAG_HEAD_SNAKE);
	snake_follow.addComponent<FollowComponent>(snake_head.getId());
}

void display(KNU::World &world) {
	for (const auto &entity : world.getEntityManager().getEntitiesByGroup(
			GROUP_SNAKE)) {
		auto positionComponent = entity.getComponent<PositionComponent>();
		log_info("ID [%d] Position [%d|%d]", entity.getId(),
				 positionComponent.y, positionComponent.x);
	}
}

int main() {
	char path[] = "/tmp/log.out";
	logger_init(path);
	KNU::World world;
	init(world);
	display(world);
	world.getSystemManager().addSystem<MotionSystem>();
	world.getSystemManager().addSystem<JoystickSystem>();
	world.getSystemManager().addSystem<FollowSystem>();


	for (int index = 0; index < 4; ++index) {
		world.update();
		world.getEventManager().emitEvent<JoystickEvent>(ARROW_DOWN);
		world.getSystemManager().getSystem<FollowSystem>()->update();
		world.getSystemManager().getSystem<JoystickSystem>()->update();
		world.getSystemManager().getSystem<MotionSystem>()->update();
		display(world);
	}
	for (int index = 0; index < 4; ++index) {
		world.update();
		world.getEventManager().emitEvent<JoystickEvent>(ARROW_RIGHT);
		world.getSystemManager().getSystem<FollowSystem>()->update();
		world.getSystemManager().getSystem<JoystickSystem>()->update();
		world.getSystemManager().getSystem<MotionSystem>()->update();
		display(world);
	}
}