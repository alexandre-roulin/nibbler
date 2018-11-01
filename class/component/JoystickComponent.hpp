#pragma once


#include <nibbler.hpp>
#include <json/json.h>

struct JoystickComponent {
private:
	static std::string const JOYSTICK;
	static std::string const TAG;
public:

	static std::string const NAME_COMPONENT;

	explicit JoystickComponent(std::string tag = "");
	JoystickComponent(Json::Value);

	eJoystick joystick;
	std::string tag;

	Json::Value serializeComponent();
};


