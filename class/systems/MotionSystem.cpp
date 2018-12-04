#include <logger.h>

#include "MotionSystem.hpp"
#include <component/PositionComponent.hpp>
#include <component/MotionComponent.hpp>
#include <KINU/World.hpp>
//P 0000 0001
//M 0000 0010

//E 0000 1111

//S 0000 0011

MotionSystem::MotionSystem() {
	requireComponent<PositionComponent>();
	requireComponent<MotionComponent>();
}

void MotionSystem::update() {
	unsigned int mapSize = getWorld().getUnivers().getMapSize();
	for (auto &entity : getEntities()) {
		auto &positionComponent = entity.getComponent<PositionComponent>();
		auto &motionComponent = entity.getComponent<MotionComponent>();
		switch (motionComponent.direction) {
			case NORTH:
				(positionComponent.y == 0 ? positionComponent.y = mapSize - 1 : positionComponent.y--);
				break;
			case SOUTH:
				(positionComponent.y == mapSize - 1 ? positionComponent.y = 0 : ++positionComponent.y);
				break;
			case EAST:
				(positionComponent.x == mapSize - 1 ? positionComponent.x = 0 : ++positionComponent.x);
				break;
			case WEST:
				(positionComponent.x == 0 ? positionComponent.x = mapSize - 1 : positionComponent.x--);
				break;
		}
	}
}

MotionSystem::~MotionSystem() {

}

