#include "MotionSystem.hpp"
#include <component/PositionComponent.hpp>
#include <component/MotionComponent.hpp>
#include <KINU/World.hpp>

MotionSystem::MotionSystem(Univers &univers) : univers_(univers) {
	requireComponent<PositionComponent>();
	requireComponent<MotionComponent>();
}

void MotionSystem::update() {
	unsigned int mapSize = univers_.getMapSize();
	for (auto &entity : getEntities()) {
		auto &positionComponent = entity.getComponent<PositionComponent>();
		auto &motionComponent = entity.getComponent<MotionComponent>();
		switch (motionComponent.direction) {
			case kNorth:
				(positionComponent.y <= 0 ? positionComponent.y = mapSize - 1 : positionComponent.y--);
				break;
			case kSouth:
				(positionComponent.y >= static_cast<int>(mapSize - 1) ? positionComponent.y = 0 : ++positionComponent.y);
				break;
			case kEast:
				(positionComponent.x >= static_cast<int>(mapSize - 1) ? positionComponent.x = 0 : ++positionComponent.x);
				break;
			case kWest:
				(positionComponent.x <= 0 ? positionComponent.x = mapSize - 1 : positionComponent.x--);
				break;
		}
	}
}

MotionSystem::~MotionSystem() = default;

