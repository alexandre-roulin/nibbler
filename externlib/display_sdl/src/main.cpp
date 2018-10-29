#include <iostream>

#include "DisplaySdl.hpp"
#include "Grid.tpp"

int				main(int argc, char **argv)
{
	static_cast<void>(argc);
	static_cast<void>(argv);

	DisplaySdl sdl("../commun/tileset1.png", 32, 30, 30, "lol");
	Grid<int> grid(30, 30);

	grid.fill(0);
	grid.setBorder(1);

	sdl.setBackground(grid);

	while (!sdl.exit())
	{
		sdl.update();
		std::cout << sdl.getDirection() << std::endl;

		sdl.drawTileGrid(1, 0, 0);
		sdl.drawTileGrid(2, 1, 0);
		sdl.drawTileGrid(3, 2, 0);
		sdl.drawTileGrid(4, 3, 0);
		sdl.drawTileGrid(5, 4, 0);

		grid(0, 0) = 1;
		grid(1, 0) = 2;

		sdl.drawTilePixel(8, 1*32+16, 1*32-16);
		sdl.render();
	}
	return (0);
}
