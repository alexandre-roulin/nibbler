#pragma once


#include <nibbler.hpp>
#include <json/json.h>

struct JoystickComponent {
private:
	static std::string const JOYSTICK;
public:

	static std::string const NAME_COMPONENT;

	explicit JoystickComponent();
	JoystickComponent(Json::Value);
	eJoystick joystick;
	Json::Value serializeComponent();
};


