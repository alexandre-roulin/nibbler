#include <logger.h>
#include <KINU/World.hpp>
#include "JoystickSystem.hpp"
#include <component/JoystickComponent.hpp>
#include <component/MotionComponent.hpp>
#include <network/SnakeClient.hpp>

JoystickSystem::JoystickSystem() {
	requireComponent<JoystickComponent>();
	requireComponent<MotionComponent>();
}

void JoystickSystem::update() {
	std::array<Snake, SNAKE_MAX> snake_array = getWorld().getUnivers().getSnakeClient()->getSnakeArray_();

	for (auto &snake : snake_array) {
		if (snake.id != -1 && getWorld().getEntitiesManager().hasEntityByTagId(snake.id + eTag::HEAD_TAG)) {
			auto entity = getWorld().getEntitiesManager().getEntityByTagId(snake.id + eTag::HEAD_TAG);
			if (entity.hasComponent<JoystickComponent>()) {
				entity.getComponent<JoystickComponent>().direction = snake.direction;
			}
		}
	}

	for (auto &entity : getEntities()) {
		auto &motionComponent = entity.getComponent<MotionComponent>();
		auto &joystickComponent = entity.getComponent<JoystickComponent>();

		if ((motionComponent.direction & DIRECTION_HORIZONTAL &&
			 !(joystickComponent.direction & DIRECTION_HORIZONTAL)) ||
			(motionComponent.direction & DIRECTION_VERTICAL &&
			 !(joystickComponent.direction & DIRECTION_VERTICAL))) {
			motionComponent.direction = joystickComponent.direction;
		}
	}

}

JoystickSystem::~JoystickSystem() = default;
