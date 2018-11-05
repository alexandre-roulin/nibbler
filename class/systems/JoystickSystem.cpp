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
	std::cout << "Size" << events.size() << std::endl;
	for (auto &event : events) {
		auto entity = getWorld().getEntityManager().getEntityByTag(event.tag_player);
		std::cout << "Entity event > " << entity.getTag() << " ID:" << entity.getId() << std::endl;
		auto &joystickComponent = entity.getComponent<JoystickComponent>();
		joystickComponent.direction = event.direction;
	}
	for (auto &entity : getEntities()) {
		auto &motionComponent = entity.getComponent<MotionComponent>();
		auto &joystickComponent = entity.getComponent<JoystickComponent>();
		motionComponent.direction = joystickComponent.direction;
	}
}

JoystickSystem::~JoystickSystem() {

}
