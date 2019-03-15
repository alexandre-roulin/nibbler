#pragma once

#include <string>
#include <ostream>
#include "IDisplay.hpp"
#include <boost/array.hpp>

#ifndef NIBBLER_ROOT_PROJECT_PATH
	#define NIBBLER_ROOT_PROJECT_PATH ""
#endif
#ifndef NIBBLER_BUILD_PROJECT_PATH
	#define NIBBLER_BUILD_PROJECT_PATH ""
#endif

#define SNAKE_MAX 8
#define CHAT_BUFFER 128
#define NAME_BUFFER 22
#define OFFSET 8

#define DIRECTION_VERTICAL 1            // 0000 0001
#define DIRECTION_HORIZONTAL 4            // 0000 0100

#define MAP_MIN 5
#define MAP_DEFAULT 30
#define MAP_MAX 100

class Snake;
class BaseSnake;
class SnakeUI;
class SnakeUX;

using SnakeArrayContainer = std::array<Snake, SNAKE_MAX>;
using SnakeArrayPtr = std::shared_ptr<SnakeArrayContainer>;
using DirectionArray = std::array<eDirection, SNAKE_MAX>;

enum eScore : uint16_t {
	kFromTime = 1,
	kFromSnakeFood = 8,
	kFromFood = 10,

};

enum eFlag {
	kSound
};

enum eTag {
	kHeadTag = 0,
	kTailTag = 8,
	kFoodTag = 16,
	kFoodFromSnake = 32,
	kWallTag = 64,
};

enum ePriority {
	kNoPriority,
	kMinorPriority,
	kSpecificLast,
};