#include <logger.h>
#include <KINU/World.hpp>
#include "JoystickSystem.hpp"
#include <component/JoystickComponent.hpp>
#include <component/MotionComponent.hpp>
#include <events/JoystickEvent.hpp>
#include <network/ClientTCP.hpp>
JoystickSystem::JoystickSystem() {
	requireComponent<JoystickComponent>();
	requireComponent<MotionComponent>();
}

void JoystickSystem::update() {
	auto events = getWorld().getEventManager().getEvents<JoystickEvent>();
	log_success("JoystickSystem::update on %d events", events.size());
	Snake const *snake_array = getWorld().getUnivers().getClientTCP_().getSnakes();
	for (int index = 0; index < MAX_SNAKE; ++index) {
		if (snake_array[index].id != -1) {
			if (getWorld().getEntityManager().hasTag(
					Factory::factory_name(HEAD, snake_array[index].id))) {
				auto entity = getWorld().getEntityManager().getEntityByTag(
						Factory::factory_name(HEAD, snake_array[index].id));
				if (entity.isAlive() && entity.hasComponent<JoystickComponent>()) {
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
			log_error("MatchDirection!");
			motionComponent.direction = joystickComponent.direction;
		}
	}

}

JoystickSystem::~JoystickSystem() {

}
