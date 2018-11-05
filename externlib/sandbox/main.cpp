#include "IDisplay.hpp"
#include <dlfcn.h>
#include <iostream>
#include <unistd.h>
bool        dlError(void)
{
    std::cerr << "Error : " << dlerror() << std::endl;
    return (false);
}

int         main(int argc, char **argv)
{
    static_cast<void>(argc);
    static_cast<void>(argv);

    void *dlHandle;

    if (!(dlHandle = dlopen(argv[1], RTLD_LAZY | RTLD_LOCAL)))
        return (dlError());

    IDisplay    *(*newDisplay)(char const *, int, int, int, char const *);
    void        (*deleteDisplay)(IDisplay *);
    if (!(newDisplay = (IDisplay *(*)(char const *, int, int, int, char const *))(dlsym(dlHandle, "newDisplay"))))
        return (dlError());
    if (!(deleteDisplay = (void (*)(IDisplay *))(dlsym(dlHandle, "deleteDisplay"))))
        return (dlError());

    IDisplay    *display = newDisplay("../commun/tileset_3snake.png", 32, 30, 30, "lol");

    Grid<int> grid(30, 30);

	grid.fill(0);
	grid.setBorder(1);

	display->setBackground(grid);

	Grid<int> map(30, 30);

	map.clear();


    while (!display->exit())
    {
        display->update();
        std::cout << display->getDirection() << std::endl;
        display->drawTileGrid(0, 0, 0);
        display->drawTileGrid(1, 1, 1);
		display->drawTileGrid(2, 2, 2);
		display->drawTileGrid(0+5*0, 17, 3, 3);
		display->drawTileGrid(1+5*0, 17, 4, 3);
        display->drawTileGrid(2+5*0, 17, 5, 3);
		display->drawTileGrid(0+5*1, 17, 3, 4);
		display->drawTileGrid(1+5*1, 17, 4, 4);
		display->drawTileGrid(2+5*1, 17, 5, 4);
		display->drawTileGrid(0+5*2, 17, 3, 5);
		display->drawTileGrid(1+5*2, 17, 4, 5);
		display->drawTileGrid(2+5*2, 17, 5, 5);
        display->drawTilePixel(12, 2*32+16, 2*32+16);


		display->drawGrid(map);

        display->render();

		map.clear();

		for(int i = 0; i < 5; ++i)
			for (int j = 0; j < 5; j++)
				map(rand()%30, rand()%30) = rand()%40;

		sleep(1);
    }
    deleteDisplay(display);
    return (0);

}
