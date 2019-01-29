#pragma once

#include "IDisplay.hpp"
#include <iostream>

class Display2D {
public:
	Display2D() {};
	virtual ~Display2D() {};

	static void debugSpriteSnake(eSprite sprite) {
		eSprite from = (sprite & eSprite::MASK_FROM) >> eSprite::BITWISE_FROM;
		eSprite to = (sprite & eSprite::MASK_TO) >> eSprite::BITWISE_TO;

		std::cout << "Entity : [";

		if ((sprite & eSprite::MASK_BODY) == eSprite::HEAD)
			std::cout << "HEAD";
		if ((sprite & eSprite::MASK_BODY) == eSprite::BODY)
			std::cout << "BODY";
		if ((sprite & eSprite::MASK_BODY) == eSprite::TAIL)
			std::cout << "TAIL";

		std::cout << "] FROM [";
		if ((from & eSprite::NORTH) == eSprite::NORTH)
			std::cout << "North";
		if ((from & eSprite::SOUTH) == eSprite::SOUTH)
			std::cout << "SOUTH";
		if ((from & eSprite::WEST) == eSprite::WEST)
			std::cout << "WEST";
		if ((from & eSprite::EAST) == eSprite::EAST)
			std::cout << "EAST";
		std::cout << "]TO [";
		if ((to & eSprite::NORTH) == eSprite::NORTH)
			std::cout << "North";
		if ((to & eSprite::SOUTH) == eSprite::SOUTH)
			std::cout << "SOUTH";
		if ((to & eSprite::WEST) == eSprite::WEST)
			std::cout << "WEST";
		if ((to & eSprite::EAST) == eSprite::EAST)
			std::cout << "EAST";
		std::bitset<32> c(static_cast<int>(sprite));

		std::cout << "::  : [" << c << "]" << std::endl;
	}


	static int getSpriteSnake(eSprite sprite) {

		eSprite from = (sprite & eSprite::MASK_FROM) >> eSprite::BITWISE_FROM;
		eSprite to = (sprite & eSprite::MASK_TO) >> eSprite::BITWISE_TO;

		if ((sprite & eSprite::WALL) == eSprite::WALL)
			return (SPRITE_WALL);
		if ((sprite & eSprite::GROUND) == eSprite::GROUND)
			return (SPRITE_GROUND);
		if ((sprite & eSprite::FOOD) == eSprite::FOOD) {
			return (SPRITE_FOOD);
		}
		if ((sprite & eSprite::MASK_BODY) == eSprite::HEAD)
			return (SIZE_LINE_TILESET *
					(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 5 +
					direction_(to));
		if ((sprite & eSprite::MASK_BODY) == eSprite::BODY) {

			if (from == to && (from == eSprite::NORTH || from == eSprite::SOUTH))
				return (SIZE_LINE_TILESET *
						(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 14);
			else if (from == to && (from == eSprite::EAST || from == eSprite::WEST))
				return (SIZE_LINE_TILESET *
						(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 13);

			else if (from == eSprite::NORTH && to == eSprite::EAST)
				return (SIZE_LINE_TILESET *
						(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 1);
			else if (from == eSprite::NORTH && to == eSprite::WEST)
				return (SIZE_LINE_TILESET *
						(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 2);

			else if (from == eSprite::SOUTH && to == eSprite::EAST)
				return (SIZE_LINE_TILESET *
						(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 3);
			else if (from == eSprite::SOUTH && to == eSprite::WEST)
				return (SIZE_LINE_TILESET *
						(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 4);

			else if (from == eSprite::WEST && to == eSprite::SOUTH)
				return (SIZE_LINE_TILESET *
						(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 1);
			else if (from == eSprite::EAST && to == eSprite::SOUTH)
				return (SIZE_LINE_TILESET *
						(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 2);

			else if (from == eSprite::WEST && to == eSprite::NORTH)
				return (SIZE_LINE_TILESET *
						(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 3);
			else if (from == eSprite::EAST && to == eSprite::NORTH)
				return (SIZE_LINE_TILESET *
						(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 4);

			else
				return (SIZE_LINE_TILESET);
		}
		if ((sprite & eSprite::MASK_BODY) == eSprite::TAIL)
			return (SIZE_LINE_TILESET *
					(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 9 +
					direction_(to));

		return (0);
	}

private:

	static int direction_(eSprite to) {
		if ((to & eSprite::SOUTH) == eSprite::SOUTH)
			return (0);
		if ((to & eSprite::WEST) == eSprite::WEST)
			return (1);
		if ((to & eSprite::NORTH) == eSprite::NORTH)
			return (2);
		return (3);
	}

	Display2D(Display2D const &src) = delete;
	Display2D &operator=(Display2D const &src) = delete;
};