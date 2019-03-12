#pragma once

#include <ostream>
#include <nibbler.hpp>

struct MotionComponent {
public:

	MotionComponent(eDirection direction = kNorth);

	eDirection direction;
};