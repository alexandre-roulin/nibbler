

#include "MotionComponent.hpp"
#include <KNU/utils/Signature.hpp>
#include <KNU/component/Component.hpp>

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

MotionComponent::MotionComponent(eDirection direction, unsigned int speed)
		: direction(direction),
		  speed(speed) {}

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
