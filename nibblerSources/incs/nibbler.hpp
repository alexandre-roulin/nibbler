#pragma once

#include <string>
#include <ostream>
#include "IDisplay.hpp"

#ifndef NIBBLER_ROOT_PROJECT_PATH
	#define NIBBLER_ROOT_PROJECT_PATH 0
#endif

#define SNAKE_MAX 8
#define SNAKE_MAX_NAME 8
#define SNAKE_MAX_COLOR 8
#define CHAT_BUFFER 128
#define NAME_BUFFER 22
#define OFFSET 8
#define SIZEOF_CHAT_PCKT CHAT_BUFFER + NAME_BUFFER + OFFSET

#define DIRECTION_VERTICAL 1            // 0000 0001
#define DIRECTION_HORIZONTAL 4            // 0000 0100

#define MAP_MIN 5
#define MAP_DEFAULT 30
#define MAP_MAX 100

class Snake;

using SnakeArrayContainer = std::array<Snake, SNAKE_MAX>;

enum eScore : uint16_t {
	kFromTime = 1,
	kFromSnakeFood = 8,
	kFromFood = 10,

};

enum eFlag {
	kSound
};

struct LibraryInfo {
	char const title[80];
	char const path[PATH_MAX];
};

enum eSound {
	kSoundSfmlLibrary = 0,
	kSoundSdlLibrary
};

static const eSound eSoundArray[] = {
		kSoundSfmlLibrary,
		kSoundSdlLibrary
};

enum eDisplay {
	kDisplaySfmlLibrary = 0,
	kDisplaySdlLibrary,
	kDisplayGlfwLibrary
};

static const eDisplay eDisplayArray[] = {
		kDisplaySfmlLibrary,
		kDisplaySdlLibrary,
		kDisplayGlfwLibrary
};

enum eTag {
	kHeadTag = 0,
	kTailTag = 8,
	kFoodTag = 16,
	kFoodFromSnake = 32,
	kWallTag = 64,
};

enum class eNoise {
	kWelcomeSound = 0,
	kReadySound = 2,
	kResizeSound = 0,
	kFoodSound = 3,
	kDeathSound = 4
};


enum ePriority {
	kNoPriority,
	kMinorPriority,
	kSpecificLast,
};