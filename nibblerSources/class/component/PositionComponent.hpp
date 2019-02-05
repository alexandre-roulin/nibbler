#pragma once

#include <ostream>

struct PositionComponent {
public:

	PositionComponent(int x = -1, int y = -1);
	PositionComponent(PositionComponent const &);
	PositionComponent(std::pair<size_t, size_t >);
	int x, y;


	operator bool() const;
	friend std::ostream &
	operator<<(std::ostream &os, const PositionComponent &positionComponent);

	PositionComponent &
	operator=(PositionComponent const &positionComponent);

	bool operator==(const PositionComponent &rhs) const;

	bool operator!=(const PositionComponent &rhs) const;

};

