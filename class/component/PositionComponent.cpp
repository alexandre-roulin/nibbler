#include <cassert>

#include <logger.h>
#include <iostream>
#include "PositionComponent.hpp"


std::string const PositionComponent::NAME_COMPONENT = "positionComponent";
std::string const PositionComponent::POS_Y = "y";
std::string const PositionComponent::POS_X = "x";


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

Json::Value PositionComponent::serializeComponent() {
	Json::Value component;
	component[POS_Y] = y;
	component[POS_X] = x;
	return component;
}

PositionComponent::PositionComponent(Json::Value json) {
	assert(json[POS_Y].isInt());
	assert(json[POS_X].isInt());
	x = json.get(POS_X, 0).asInt();
	y = json.get(POS_Y, 0).asInt();
}

PositionComponent &
PositionComponent::operator=(PositionComponent const &motionComponent) {
	if (this != &motionComponent) {
		y = motionComponent.y;
		x = motionComponent.x;
	}
	return *this;
}

