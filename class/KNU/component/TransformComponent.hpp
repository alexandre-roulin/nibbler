#pragma once
#include "Component.hpp"

namespace KNU {

	struct TransformComponent {
		explicit TransformComponent(int y = 0, int x = 0) : y(y), x(x) {};
		int y, x;
	};
}

