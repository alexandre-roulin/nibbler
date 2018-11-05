
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
		auto &positionComponent = entity.getComponent<PositionComponent>();
		auto &motionComponent = entity.getComponent<MotionComponent>();
		std::cout << "Pre: "  << positionComponent << std::endl;

		switch (motionComponent.direction) {
			case NORTH:
				--positionComponent.y;
				break;
			case SOUTH:
				++positionComponent.y;
				break;
			case EAST:
				++positionComponent.x;
				break;
			case WEST:
				--positionComponent.x;
				break;
		}
		std::cout << "Post: " << positionComponent << std::endl;
	}
}

MotionSystem::~MotionSystem() {

}
