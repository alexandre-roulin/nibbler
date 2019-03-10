#pragma once

#include "IDisplay.hpp"
#include <iostream>

class Display2D {
public:
	Display2D() = default;
	virtual ~Display2D() = default;
	Display2D &operator=(const Display2D &) = delete;
	Display2D(const Display2D &) = delete;

	static void debugSpriteSnake(eSprite sprite) {
		eSprite from = (sprite & eSprite::kMaskFrom) >> eSprite::kBitwiseFrom;
		eSprite to = (sprite & eSprite::kMaskTo) >> eSprite::kBitwiseTo;

		std::cout << "Entity : [";

		if ((sprite & eSprite::kMaskBody) == eSprite::kHead)
			std::cout << "HEAD";
		if ((sprite & eSprite::kMaskBody) == eSprite::kBody)
			std::cout << "BODY";
		if ((sprite & eSprite::kMaskBody) == eSprite::kTail)
			std::cout << "TAIL";

		std::cout << "] FROM [";
		if ((from & eSprite::kNorth) == eSprite::kNorth)
			std::cout << "North";
		if ((from & eSprite::kSouth) == eSprite::kSouth)
			std::cout << "SOUTH";
		if ((from & eSprite::kWest) == eSprite::kWest)
			std::cout << "WEST";
		if ((from & eSprite::kEast) == eSprite::kEast)
			std::cout << "EAST";
		std::cout << "]TO [";
		if ((to & eSprite::kNorth) == eSprite::kNorth)
			std::cout << "North";
		if ((to & eSprite::kSouth) == eSprite::kSouth)
			std::cout << "SOUTH";
		if ((to & eSprite::kWest) == eSprite::kWest)
			std::cout << "WEST";
		if ((to & eSprite::kEast) == eSprite::kEast)
			std::cout << "EAST";
		std::bitset<32> c(static_cast<int>(sprite));

		std::cout << "::  : [" << c << "]" << std::endl;
	}

	static int getSpriteSnakeByColor(eSprite color, int part) {
		return (SIZE_LINE_TILESET *
				(static_cast<int>(color & eSprite::kMaskColor) - 1) + part);
	}

	static int getSpriteSnake(eSprite sprite) {

		eSprite from = (sprite & eSprite::kMaskFrom) >> eSprite::kBitwiseFrom;
		eSprite to = (sprite & eSprite::kMaskTo) >> eSprite::kBitwiseTo;

		if ((sprite & eSprite::kWall) == eSprite::kWall)
			return (SPRITE_WALL);
		if ((sprite & eSprite::kTree) == eSprite::kTree)
			return (SPRITE_TREE);
		if ((sprite & eSprite::kGround) == eSprite::kGround)
			return (SPRITE_GROUND);
		if ((sprite & eSprite::kFood) == eSprite::kFood) {
			return (SPRITE_FOOD);
		}
		if ((sprite & eSprite::kMaskBody) == eSprite::kHead)
			return getSpriteSnakeByColor(sprite, 5 + direction_(to));
		if ((sprite & eSprite::kMaskBody) == eSprite::kBody) {

			if (from == to && (from == eSprite::kNorth || from == eSprite::kSouth))
				return getSpriteSnakeByColor(sprite, 14);
			else if (from == to && (from == eSprite::kEast || from == eSprite::kWest))
				return getSpriteSnakeByColor(sprite, 13);
			else if (from == eSprite::kNorth && to == eSprite::kEast)
				return getSpriteSnakeByColor(sprite, 1);
			else if (from == eSprite::kNorth && to == eSprite::kWest)
				return getSpriteSnakeByColor(sprite, 2);
			else if (from == eSprite::kSouth && to == eSprite::kEast)
				return getSpriteSnakeByColor(sprite, 3);
			else if (from == eSprite::kSouth && to == eSprite::kWest)
				return getSpriteSnakeByColor(sprite, 4);
			else if (from == eSprite::kWest && to == eSprite::kSouth)
				return getSpriteSnakeByColor(sprite, 1);
			else if (from == eSprite::kEast && to == eSprite::kSouth)
				return getSpriteSnakeByColor(sprite, 2);
			else if (from == eSprite::kWest && to == eSprite::kNorth)
				return getSpriteSnakeByColor(sprite, 3);
			else if (from == eSprite::kEast && to == eSprite::kNorth)
				return getSpriteSnakeByColor(sprite, 4);
			else
				return (SIZE_LINE_TILESET);
		}
		if ((sprite & eSprite::kMaskBody) == eSprite::kTail)
			return (SIZE_LINE_TILESET *
					(static_cast<int>(sprite & eSprite::kMaskColor) - 1) + 9 +
					direction_(to));

		return (0);
	}

private:

	static int direction_(eSprite to) {
		if ((to & eSprite::kSouth) == eSprite::kSouth)
			return (0);
		if ((to & eSprite::kWest) == eSprite::kWest)
			return (1);
		if ((to & eSprite::kNorth) == eSprite::kNorth)
			return (2);
		return (3);
	}
};