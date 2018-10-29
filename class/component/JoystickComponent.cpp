#include "JoystickComponent.hpp"


std::string const JoystickComponent::NAME_COMPONENT = "joystickComponent";
std::string const JoystickComponent::JOYSTICK = "joystick";

JoystickComponent::JoystickComponent() : joystick(NONE) {

}

Json::Value JoystickComponent::serializeComponent() {
	Json::Value component;
	component[JOYSTICK] = static_cast<int>(joystick);
	return component;
}

JoystickComponent::JoystickComponent(Json::Value json) {
//	assert(json[JOYSTICK].isInt());
	joystick = static_cast<eJoystick >(json.get(JOYSTICK, NONE).asInt());
}
