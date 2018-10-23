//
// Created by Alexandre ROULIN on 23/10/2018.
//

#ifndef NIBBLER_TRANSFORMCOMPONENT_HPP
#define NIBBLER_TRANSFORMCOMPONENT_HPP

#include <nibbler.hpp>
#include <ostream>
#include "Component.hpp"

struct TransformComponent : Component<TransformComponent> {
	TransformComponent() : Component(TRANSFORM_COMPONENT), y(0), x(0) {}

	TransformComponent(int y, int x) : Component(MOTION_COMPONENT), y(y),
									   x(x) {}

	int x;
	int y;
};

std::ostream &operator<<(std::ostream &os, const TransformComponent &mc) {
	os << " y : " << mc.x << " x : " << mc.y;
	return os;
}

#endif //NIBBLER_TRANSFORMCOMPONENT_HPP
