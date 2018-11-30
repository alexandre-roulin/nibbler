#pragma once

#include <ostream>
#include <nibbler.hpp>
#include <json/json.h>

struct MotionComponent {
public:

	MotionComponent(eDirection direction = NORTH);

	eDirection direction;
};