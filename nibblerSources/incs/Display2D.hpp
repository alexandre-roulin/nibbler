#pragma once

#include "IDisplay.hpp"
#include <iostream>

class Display2D {
public:
	Display2D() = default;
	virtual ~Display2D() = default;
	Display2D &operator=(const Display2D &) = delete;
	Display2D(const Display2D &) = delete;

	static int getSpriteSnakeByColor(eSprite color, int part);

private:
	static int direction_(eSprite to);
};
