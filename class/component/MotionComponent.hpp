#pragma once

#include <ostream>
#include <nibbler.hpp>
#include <KNU/utils/Signature.hpp>
#include <json/json.h>

struct MotionComponent {
private:
	static std::string const NAME_COMPONENT;
	static std::string const DIRECTION;
	static std::string const SPEED;

public:
	explicit MotionComponent(
			eDirection direction = NORTH,
			unsigned int speed = 1);

	eDirection direction;

	unsigned int speed;

	Json::Value serializeComponent();

	MotionComponent &operator=(MotionComponent const &motionComponent);

	friend std::ostream &
	operator<<(std::ostream &os, const MotionComponent &component);
};