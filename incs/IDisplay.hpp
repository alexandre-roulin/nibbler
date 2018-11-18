#pragma once

#include "Grid.tpp"
#include <nibbler.hpp>
#define SPRITE_GROUND 8 * SIZE_LINE_TILESET
#define SPRITE_WALL 8 * SIZE_LINE_TILESET + 1
#define SPRITE_FOOD SIZE_LINE_TILESET
#define PATH_TILESET "./ressource/snake_tileset.png"
#define SIZE_LINE_TILESET 15
#define PATH_LIBRARY_SFML "./externlib/display_sfml/libdisplay_sfml.so"
#define PATH_LIBRARY_SDL "./externlib/display_sdl/libdisplay_sdl.so"
class IDisplay {
public:
	virtual ~IDisplay() {}
    virtual bool        exit(void) const = 0;
    virtual void        render(void) = 0;
    virtual void        update(void) = 0;
    virtual void		drawTileGrid(int indexTile, int indexWidthGrid, int indexHeightGrid) = 0;
	virtual void		drawTileGrid(int indexWidthTile, int indexHeightTile, int indexWidthGrid, int indexHeightGrid) = 0;
    virtual void		drawTilePixel(int indexTile, int indexWidthPixel, int indexHeightPixel) = 0;
	virtual void		drawTilePixel(int indexWidthTile, int indexHeightTile, int indexWidthPixel, int indexHeightPixel) = 0;
	virtual void		drawGrid(Grid<int> const &grid) = 0;
	virtual void		setBackground(Grid<int> const &grid) = 0;
    virtual eDirection  getDirection(void) const = 0;
};
