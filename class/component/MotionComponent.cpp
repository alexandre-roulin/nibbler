
#include "MotionComponent.hpp"

MotionComponent &
MotionComponent::operator=(const MotionComponent &motionComponent)  {
	if (this != &motionComponent) {
		direction = motionComponent.direction;
		speed = motionComponent.speed;
		mask = motionComponent.mask;
	}
	return *this;
}

std::ostream &operator<<(std::ostream &os, const MotionComponent &component) {
	os << "direction: " << component.direction << " speed: " << component.speed
	   << " mask: " << component.mask;
	return os;
}
