#pragma once

#include <MutantGrid.tpp>
#include <Sprite.hpp>

#define SIZE_LINE_TILESET 15
#define SPRITE_GROUND 8 * SIZE_LINE_TILESET
#define SPRITE_WALL 8 * SIZE_LINE_TILESET + 1
#define SPRITE_TREE 8 * SIZE_LINE_TILESET + 1
#define SPRITE_FOOD SIZE_LINE_TILESET
#define DISPLAY_DEFAULT_TILE_SIZE 32


enum eDirection {
	kNorth = 1,							// 0000 0001
	kSouth = 3,							// 0000 0011
	kEast = 4,							// 0000 0100
	kWest = 12							// 0000 1100
};

enum eAction {
	kPause,
	kSwitchDisplayLibrary,
	kSwitchDisplayLibrarySFML,
	kSwitchDisplayLibrarySDL,
	kSwitchDisplayLibraryGLFW,
};

class IDisplay {
public:

	virtual ~IDisplay() {}

	virtual bool exit() const = 0;

	virtual void render() = 0;

	virtual void update() = 0;

	virtual void drawGrid(MutantGrid<eSprite> const &grid) = 0;

	virtual void setBackground(MutantGrid<eSprite> const &grid) = 0;

	virtual eDirection getDirection() const = 0;

	virtual void setDirection(eDirection direction) = 0;

	virtual void registerCallbackAction(std::function<void(eAction)>) = 0;
};

