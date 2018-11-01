#include "JoystickComponent.hpp"
#include <cassert>


std::string const JoystickComponent::NAME_COMPONENT = "joystickComponent";
std::string const JoystickComponent::JOYSTICK = "joystick";
std::string const JoystickComponent::TAG = "tag";

JoystickComponent::JoystickComponent(std::string tag) : joystick(NONE), tag(tag) {

}

Json::Value JoystickComponent::serializeComponent() {
	Json::Value component;
	component[JOYSTICK] = static_cast<int>(joystick);
	component[TAG] = tag;
	return component;
}

JoystickComponent::JoystickComponent(Json::Value json) {
	assert(json[JOYSTICK].isInt());
	assert(json[TAG].isString());
	joystick = static_cast<eJoystick >(json.get(JOYSTICK, NONE).asInt());
	tag = json.get(JOYSTICK, NONE).asString();
}
