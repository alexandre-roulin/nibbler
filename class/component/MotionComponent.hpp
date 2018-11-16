#pragma once

#include <ostream>
#include <nibbler.hpp>
#include <json/json.h>

struct MotionComponent {
private:
	static std::string const DIRECTION;
	static std::string const SPEED;
public:

	static std::string const NAME_COMPONENT;
	explicit MotionComponent(eDirection direction = NORTH);
	MotionComponent(Json::Value);
	eDirection direction;

	unsigned int speed;

	Json::Value serializeComponent();

	MotionComponent &operator=(MotionComponent const &motionComponent);

	friend std::ostream &
	operator<<(std::ostream &os, const MotionComponent &component);
};