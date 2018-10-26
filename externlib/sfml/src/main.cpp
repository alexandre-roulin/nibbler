#include "DisplaySfml.hpp"

int         main(int argc, char **argv)
{
    static_cast<void>(argc);
    static_cast<void>(argv);
    DisplaySfml sfml("../commun/tileset1.png", 32, 30, 30, "lol");

    Grid<int> grid(30, 30);

	grid.fill(0);
	grid.setBorder(1);

	sfml.setBackground(grid);

    while (!sfml.exit())
    {
        sfml.update();
        std::cout << sfml.getDirection() << std::endl;
        sfml.drawTileGrid(0, 0, 0);
        sfml.drawTileGrid(1, 1, 1);
        sfml.drawTileGrid(2, 2, 2);
        sfml.drawTilePixel(12, 2*32+16, 2*32+16);
        sfml.render();
    }
    return (0);
}
