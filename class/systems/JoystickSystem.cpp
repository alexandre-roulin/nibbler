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
	for (auto &entity : getEntities()) {
		auto &motionComponent = entity.getComponent<MotionComponent>();
		auto events = getWorld().getEventManager().getEvents<JoystickEvent>();
		for (auto &event : events) {
			switch (event.joystick) {
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
		}
	}
}

JoystickSystem::~JoystickSystem() {

}
