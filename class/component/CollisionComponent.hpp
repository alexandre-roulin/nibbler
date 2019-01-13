#pragma once

#include <ostream>

struct CollisionComponent {
	explicit CollisionComponent();

	friend std::ostream &
	operator<<(std::ostream &os, const CollisionComponent &component);
};


