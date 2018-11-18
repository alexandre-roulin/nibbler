
#include "MotionSystem.hpp"
#include <component/PositionComponent.hpp>
#include <component/MotionComponent.hpp>

//P 0000 0001
//M 0000 0010

//E 0000 1111

//S 0000 0011

MotionSystem::MotionSystem() {
	requireComponent<PositionComponent>();
	requireComponent<MotionComponent>();
}

void MotionSystem::update() {
	log_success("update");
	for (auto &entity : getEntities()) {
		auto &positionComponent = entity.getComponent<PositionComponent>();
		auto &motionComponent = entity.getComponent<MotionComponent>();

		log_info("Entity [%d] x [%d] y [%d] mc [%d]", entity.getIndex(),
				 positionComponent.x, positionComponent.y,
				 static_cast<int>(motionComponent.direction));

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
	}
}

MotionSystem::~MotionSystem() {

}

