#pragma once

#include <ostream>
#include <nibbler.hpp>
#include <KNU/utils/Signature.hpp>

struct MotionComponent {
public:
	explicit MotionComponent(
			eDirection direction = NORTH,
			unsigned int speed = 1);

	eDirection direction;

	unsigned int speed;

	MotionComponent &operator=(MotionComponent const &motionComponent);

	friend std::ostream &
	operator<<(std::ostream &os, const MotionComponent &component);
};