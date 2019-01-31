#include <logger.h>
#include <KINU/World.hpp>
#include "JoystickSystem.hpp"
#include <component/JoystickComponent.hpp>
#include <component/MotionComponent.hpp>
#include <network/ClientTCP.hpp>

JoystickSystem::JoystickSystem() {
	requireComponent<JoystickComponent>();
	requireComponent<MotionComponent>();
}

void JoystickSystem::update() {
	Snake const *snake_array = getWorld().getUnivers().getGameNetwork()->getSnakes();

	for (int index = 0; index < MAX_SNAKE; ++index) {
		if (snake_array[index].id != -1) {
			if (getWorld().getEntitiesManager().hasEntityByTagId(
					snake_array[index].id + eTag::HEAD_TAG)) {
				auto entity = getWorld().getEntitiesManager().
						getEntityByTagId(snake_array[index].id + eTag::HEAD_TAG);
				if (entity.hasComponent<JoystickComponent>()) {
					auto &joystickComponent = entity.getComponent<JoystickComponent>();
					joystickComponent.direction = snake_array[index].direction;
				}
			}
		}
	}
	for (auto &entity : getEntities()) {
		auto &motionComponent = entity.getComponent<MotionComponent>();
		auto &joystickComponent = entity.getComponent<JoystickComponent>();

//		log_error(
//				"Match Mot[%d] Joy[%d] MotHor[%d] JoyHor[%d] MotVer[%d] JoyVer[%d]",
//				motionComponent.direction,
//				joystickComponent.direction,
//				motionComponent.direction & DIRECTION_HORIZONTAL,
//				joystickComponent.direction & DIRECTION_HORIZONTAL,
//				motionComponent.direction & DIRECTION_VERTICAL,
//				joystickComponent.direction & DIRECTION_VERTICAL
//		);

		if ((motionComponent.direction & DIRECTION_HORIZONTAL &&
			 !(joystickComponent.direction & DIRECTION_HORIZONTAL)) ||
			(motionComponent.direction & DIRECTION_VERTICAL &&
			 !(joystickComponent.direction & DIRECTION_VERTICAL))) {
//			log_error("MatchDirection!");
//			log_info("Entity 0 size %d", getWorld().getEntitiesManager().getEntitiesByGroupId(0).size());
//			log_info("Entity 1 size %d", getWorld().getEntitiesManager().getEntitiesByGroupId(1).size());
			motionComponent.direction = joystickComponent.direction;
		}
	}

}

JoystickSystem::~JoystickSystem() {

}
