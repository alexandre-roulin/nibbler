#include <logger.h>

#include "MotionSystem.hpp"
#include <component/PositionComponent.hpp>
#include <component/MotionComponent.hpp>
#include <KINU/World.hpp>
//P 0000 0001
//M 0000 0010

//E 0000 1111

//S 0000 0011

MotionSystem::MotionSystem(Univers &univers) : univers_(univers) {
	requireComponent<PositionComponent>();
	requireComponent<MotionComponent>();
}

void MotionSystem::update() {
	static int frame = 0;
	log_info("%s", __PRETTY_FUNCTION__);
	unsigned int mapSize = univers_.getMapSize();
	for (auto &entity : getEntities()) {
		auto &positionComponent = entity.getComponent<PositionComponent>();
		auto &motionComponent = entity.getComponent<MotionComponent>();
//		std::cout << "On frame : " << frame << "_ Entity : " << entity.getId() << " _ Direction : " << motionComponent.direction << std::endl;
		switch (motionComponent.direction) {
			case kNorth:
				(positionComponent.y == 0 ? positionComponent.y = mapSize - 1 : positionComponent.y--);
				break;
			case kSouth:
				(positionComponent.y == static_cast<int>(mapSize - 1) ? positionComponent.y = 0 : ++positionComponent.y);
				break;
			case kEast:
				(positionComponent.x == static_cast<int>(mapSize - 1) ? positionComponent.x = 0 : ++positionComponent.x);
				break;
			case kWest:
				(positionComponent.x == 0 ? positionComponent.x = mapSize - 1 : positionComponent.x--);
				break;
		}
	}
	frame++;
//	std::cout << std::endl;
}

MotionSystem::~MotionSystem() = default;

