#include <cassert>

#include <logger.h>
#include <iostream>
#include "PositionComponent.hpp"


PositionComponent::PositionComponent(int y, int x) : y(y), x(x) {
}

bool PositionComponent::operator==(const PositionComponent &rhs) const {
	return y == rhs.y && x == rhs.x;
}

bool PositionComponent::operator!=(const PositionComponent &rhs) const {
	return !(rhs == *this);
}

PositionComponent::PositionComponent(PositionComponent const &positionComponent) {
	*this = positionComponent;
}

PositionComponent &
PositionComponent::operator=(PositionComponent const &motionComponent) {
	if (this != &motionComponent) {
		y = motionComponent.y;
		x = motionComponent.x;
	}
	return *this;
}

