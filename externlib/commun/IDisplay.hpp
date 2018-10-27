#pragma once
#include "Grid.tpp"

enum eDirection { NONE, NORTH, SOUTH, EAST, WEST };

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
