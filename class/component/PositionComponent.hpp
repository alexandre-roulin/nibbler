#pragma once

#include <ostream>

struct PositionComponent {
public:

	PositionComponent(int x = 0, int y = 0);
	PositionComponent(PositionComponent const &);
	PositionComponent(std::pair<size_t, size_t >);
	int y, x;


	PositionComponent &
	operator=(PositionComponent const &motionComponent);

	bool operator==(const PositionComponent &rhs) const;

	bool operator!=(const PositionComponent &rhs) const;

};

