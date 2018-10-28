#pragma once


#include <nibbler.hpp>
#include <json/json.h>

struct JoystickComponent {
private:
	static std::string const NAME_COMPONENT;
	static std::string const JOYSTICK;

public:

	explicit JoystickComponent();
	eJoystick joystick;
	Json::Value serializeComponent();
};


