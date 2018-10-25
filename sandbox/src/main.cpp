#include <iostream>

#include "DisplaySdl.hpp"
#include "Grid.tpp"

int				main(int argc, char **argv)
{
	static_cast<void>(argc);
	static_cast<void>(argv);

	DisplaySdl sdl("tileset1.png", 32, 30, 30, "lol");


	Grid<int> grid(30, 30);

	grid.fill(0);
	grid.setBorder(1);

	sdl.setBackground(grid);

	while (!sdl.exit())
	{
		sdl.update();
		sdl.drawTile(1, 0, 0);
		sdl.drawTile(2, 1, 0);
		sdl.drawTile(3, 2, 0);
		sdl.drawTile(4, 3, 0);
		sdl.drawTile(5, 4, 0);
		sdl.render();
	}
	return (0);
}
