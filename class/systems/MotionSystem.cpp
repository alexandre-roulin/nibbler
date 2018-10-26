
#include <component/TransformComponent.hpp>
#include <component/MotionComponent.hpp>
#include "MotionSystem.hpp"

MotionSystem::MotionSystem() {
	requireComponent<TransformComponent>();
	requireComponent<MotionComponent>();
}

void MotionSystem::update() {
	for (auto &entity : getEntities()) {
		auto &transformComponent = entity.getComponent<TransformComponent>();
		auto &motionComponent = entity.getComponent<MotionComponent>();
		switch (motionComponent.direction) {
			case NORTH:
				transformComponent.y--;
				break;
			case SOUTH:
				transformComponent.y++;
				break;
			case EAST:
				transformComponent.x++;
				break;
			case WEST:
				transformComponent.x--;
				break;
		}
	}
}

MotionSystem::~MotionSystem() {

}
