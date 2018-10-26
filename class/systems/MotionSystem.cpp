
#include <component/TransformComponent.hpp>
#include <component/MotionComponent.hpp>
#include <logger.h>
#include "MotionSystem.hpp"

MotionSystem::MotionSystem() {
	requireComponent<TransformComponent>();
	requireComponent<MotionComponent>();
}

void MotionSystem::update() {
	log_success("MotionSystem::update SizeEntities [%d]", getEntities().size());
	for (auto &entity : getEntities()) {
		auto &transformComponent = entity.getComponent<TransformComponent>();
		auto &motionComponent = entity.getComponent<MotionComponent>();
		switch (motionComponent.direction) {
			case NORTH:
				transformComponent.y -= motionComponent.speed;
				break;
			case SOUTH:
				transformComponent.y += motionComponent.speed;
				break;
			case EAST:
				transformComponent.x += motionComponent.speed;
				break;
			case WEST:
				transformComponent.x -= motionComponent.speed;
				break;
		}
	}
}

MotionSystem::~MotionSystem() {

}
