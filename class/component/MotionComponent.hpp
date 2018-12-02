#pragma once

#include <ostream>
#include <nibbler.hpp>

struct MotionComponent {
public:

	MotionComponent(eDirection direction = NORTH);

	eDirection direction;
};