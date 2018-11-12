#include "JoystickComponent.hpp"
#include <cassert>

std::string const JoystickComponent::NAME_COMPONENT = "joystickComponent";
std::string const JoystickComponent::DIRECTION = "direction";
std::string const JoystickComponent::TAG = "tag";

JoystickComponent::JoystickComponent(eDirection direction, std::string tag)
		: direction(direction), tag(tag) {

}

Json::Value JoystickComponent::serializeComponent() {
	Json::Value component;
	component[DIRECTION] = static_cast<int>(direction);
	component[TAG] = tag;
	return component;
}

JoystickComponent::JoystickComponent(Json::Value json) {
	assert(json[DIRECTION].isInt());
	assert(json[TAG].isString());
	direction = static_cast<eDirection >(json.get(DIRECTION, NORTH).asInt());
	tag = json.get(DIRECTION, NORTH).asString();
}
