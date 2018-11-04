#pragma once


#include <nibbler.hpp>
#include <json/json.h>

struct JoystickComponent {
private:
	static std::string const DIRECTION;
	static std::string const TAG;
public:

	static std::string const NAME_COMPONENT;

	explicit JoystickComponent(eDirection direction = NORTH, std::string tag = "");
	JoystickComponent(Json::Value);

	eDirection direction;
	std::string tag;

	Json::Value serializeComponent();
};


