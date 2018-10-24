#pragma once

#include <ostream>
#include "Component.hpp"

namespace KNU {

	struct TransformComponent {
		explicit TransformComponent(int y = 0, int x = 0) : y(y), x(x) {};
		int y, x;


	};

//	std::ostream &
//	operator<<(std::ostream &os, const TransformComponent &component) {
//		os << "y: " << component.y << " x: " << component.x;
//		return os;
//	}


};
