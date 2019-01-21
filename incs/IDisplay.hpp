#pragma once

#define SPRITE_GROUND 8 * SIZE_LINE_TILESET
#define SPRITE_WALL 8 * SIZE_LINE_TILESET + 1
#define SPRITE_FOOD SIZE_LINE_TILESET
#define PATH_TILESET "./ressource/snake_tileset.png"
#define SIZE_LINE_TILESET 15
//#define PATH_DISPLAY_LIBRARY_SFML "./extern/display_sfml/libdisplay_sfml.so"
#define PATH_DISPLAY_LIBRARY_SFML "./extern/test_glfw/libGlitter.so"
//#define PATH_DISPLAY_LIBRARY_SFML "./extern/display_sdl/libdisplay_sdl.so"

#define PATH_DISPLAY_LIBRARY_SDL "./extern/display_sdl/libdisplay_sdl.so"
#define PATH_SOUND_LIBRARY_SFML "./extern/sound_sfml/libsound_sfml.so"
#define PATH_SOUND_LIBRARY_SDL "./extern/sound_sdl/libsound_sdl.so"
#define MUSIC_ZELDA "./ressource/sound/zelda.ogg"

#define DISPLAY_DEFAULT_TILESET_PATH "snake_tileset.png"
#define DISPLAY_DEFAULT_TILE_SIZE 32

#include <Grid.tpp>
#include <Sprite.hpp>

#ifdef _WIN32
#define DISPLAY_SLASH "\\"
#else
#define DISPLAY_SLASH "/"
#endif

enum eDirection {
	NORTH = 1,							// 0000 0001
	SOUTH = 3,							// 0000 0011
	EAST = 4,							// 0000 0100
	WEST = 12							// 0000 1100
};

class IDisplay {
public:

	virtual ~IDisplay() {}

	virtual bool exit(void) const = 0;

	virtual void render(float currentDelayFrame, float maxDelayFrame) = 0;

	virtual void update(float deltaTime) = 0;

	virtual void drawGrid(Grid<eSprite> const &grid) = 0;

	virtual void setBackground(Grid<eSprite> const &grid) = 0;

	virtual eDirection getDirection(void) const = 0;
};

