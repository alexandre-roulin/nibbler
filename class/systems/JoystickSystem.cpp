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
	log_success("update");
	auto events = getWorld().getEventManager().getEvents<JoystickEvent>();
//	std::cout << "Size" << events.size() << std::endl;
	for (auto &event : events) {
		auto entity = getWorld().getEntityManager().getEntityByTag(event.tag_player);

		if (entity.getId() != -1 && entity.hasComponent<JoystickComponent>()) {
			auto &joystickComponent = entity.getComponent<JoystickComponent>();
			joystickComponent.direction = event.direction;
		}
	}
	for (auto &entity : getEntities()) {
		auto &motionComponent = entity.getComponent<MotionComponent>();
		auto &joystickComponent = entity.getComponent<JoystickComponent>();
		motionComponent.direction = joystickComponent.direction;
	}
}

JoystickSystem::~JoystickSystem() {

}
