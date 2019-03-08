#include <KINU/World.hpp>
#include "JoystickSystem.hpp"
#include <component/JoystickComponent.hpp>
#include <component/MotionComponent.hpp>
#include <network/SnakeClient.hpp>
#include <nibbler.hpp>

JoystickSystem::JoystickSystem() {
	requireComponent<JoystickComponent>();
	requireComponent<MotionComponent>();
}

void JoystickSystem::update() {

	std::vector<DirectionArray> directions = getWorld().getEventsManager().getEvents<DirectionArray>();
	getWorld().getEventsManager().destroy<DirectionArray>();
	assert(directions.size() == 1);
	for (int id = 0; id < SNAKE_MAX; ++id) {
		if (getWorld().getEntitiesManager().hasEntityByTagId(id + eTag::kHeadTag)) {
			auto entity = getWorld().getEntitiesManager().getEntityByTagId(id + eTag::kHeadTag);
			if (entity.hasComponent<JoystickComponent>()) {
				entity.getComponent<JoystickComponent>().direction = directions[0][id];
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
