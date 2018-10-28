

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
	component[NAME_COMPONENT][SPEED] = speed;
	component[NAME_COMPONENT][DIRECTION] = direction;
	return component;
}
