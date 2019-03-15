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
	std::cout << "Begin" << std::endl;
	std::cout << directions.size() << std::endl;
	assert(directions.size() > 0);
	std::cout << directions.size() << std::endl;
	assert(directions[0].size() == SNAKE_MAX);
	for (int id = 0; id < SNAKE_MAX; ++id) {
		if (getWorld().getEntitiesManager().hasEntityByTagId(id + eTag::kHeadTag)) {
			auto entity = getWorld().getEntitiesManager().getEntityByTagId(id + eTag::kHeadTag);
			if (entity.hasComponent<JoystickComponent>()) {
				(void)entity.getComponent<JoystickComponent>();
				(void)entity.getComponent<JoystickComponent>().direction;
				(void)directions[0][id];

				std::cout << directions.size();
				std::cout << " " << directions[0][id];
				entity.getComponent<JoystickComponent>().direction = directions[0][id];
			}
		}
	}
	std::cout << std::endl;

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
