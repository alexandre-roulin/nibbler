
#include <component/PositionComponent.hpp>
#include <component/MotionComponent.hpp>
#include <logger.h>
#include "MotionSystem.hpp"

//P 0000 0001
//M 0000 0010

//E 0000 1111

//S 0000 0011

MotionSystem::MotionSystem() {
	requireComponent<PositionComponent>();
	requireComponent<MotionComponent>();
}

void MotionSystem::update() {
	log_success("MotionSystem::update SizeEntities [%d]", getEntities().size());
	for (auto &entity : getEntities()) {
		auto &transformComponent = entity.getComponent<PositionComponent>();
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
