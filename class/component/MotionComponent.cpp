

#include "MotionComponent.hpp"
#include <KINU/Component.hpp>
#include <cassert>

std::string const MotionComponent::NAME_COMPONENT = "motionComponent";
std::string const MotionComponent::SPEED = "speed";
std::string const MotionComponent::DIRECTION = "direction";


MotionComponent &
MotionComponent::operator=(const MotionComponent &motionComponent) {
	if (this != &motionComponent) {
		direction = motionComponent.direction;
		speed = motionComponent.speed;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &os, const MotionComponent &component) {
	os << "direction: " << component.direction << " speed: " << component.speed;
	return os;
}

MotionComponent::MotionComponent(eDirection direction)
		: direction(direction) {

}

Json::Value MotionComponent::serializeComponent() {
	Json::Value component;
	component[SPEED] = speed;
	component[DIRECTION] = direction;
	return component;
}

MotionComponent::MotionComponent(Json::Value json) {
	assert(json[SPEED].isInt());
	assert(json[DIRECTION].isInt());
	speed = json.get(SPEED, 1).asInt();
	direction = static_cast<eDirection>(json.get(DIRECTION, NORTH).asInt());
}
