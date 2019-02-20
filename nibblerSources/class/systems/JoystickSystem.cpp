#include <logger.h>
#include <KINU/World.hpp>
#include "JoystickSystem.hpp"
#include <component/JoystickComponent.hpp>
#include <component/MotionComponent.hpp>
#include <network/SnakeClient.hpp>

JoystickSystem::JoystickSystem(Univers &univers): univers_(univers) {
	requireComponent<JoystickComponent>();
	requireComponent<MotionComponent>();
}

void JoystickSystem::update() {
	log_info("%s", __PRETTY_FUNCTION__);
	SnakeClient::boost_shared_ptr ptr(univers_.getSnakeClient().lock());
	if (!ptr)
		return;
	SnakeArrayContainer const &snake_array = ptr->getSnakeArray_();

	for (auto &snake : snake_array) {
		if (snake.isValid && getWorld().getEntitiesManager().hasEntityByTagId(snake.id_ + eTag::kHeadTag)) {
			auto entity = getWorld().getEntitiesManager().getEntityByTagId(snake.id_ + eTag::kHeadTag);
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
