#include <component/JoystickComponent.hpp>
#include <component/MotionComponent.hpp>
#include "JoystickSystem.hpp"
#include <KNU/World.hpp>
#include <events/JoystickEvent.hpp>

JoystickSystem::JoystickSystem() {
	requireComponent<JoystickComponent>();
	requireComponent<MotionComponent>();
}

void JoystickSystem::update() {
	auto events = getWorld().getEventManager().getEvents<JoystickEvent>();
	for (auto &event : events) {
		auto entity = getWorld().getEntityManager()
				.getEntityByTag(event.tag_player);
		auto &joystickComponent = entity.getComponent<JoystickComponent>();
		joystickComponent.joystick = event.joystick;
	}
	for (auto &entity : getEntities()) {
		auto &motionComponent = entity.getComponent<MotionComponent>();
		auto &joystickComponent = entity.getComponent<JoystickComponent>();
		switch (joystickComponent.joystick) {
			case ARROW_UP:
				motionComponent.direction = NORTH;
				break;
			case ARROW_DOWN:
				motionComponent.direction = SOUTH;
				break;
			case ARROW_LEFT:
				motionComponent.direction = WEST;
				break;
			case ARROW_RIGHT:
				motionComponent.direction = EAST;
				break;
			case NONE:
				break;
		}
		joystickComponent.joystick = NONE;
	}
}

JoystickSystem::~JoystickSystem() {

}
