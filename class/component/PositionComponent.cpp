
#include <logger.h>
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
	log_error("POS_Y");
	component[NAME_COMPONENT][POS_Y] = y;
	log_error("POS_X");
	component[NAME_COMPONENT][POS_X] = x;
	log_error("NAME_COMPONENT");
	return component;
}

