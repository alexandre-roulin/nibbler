#include "Core.hpp"
#include "IDisplay.hpp"
#include <dlfcn.h>
#include <iostream>

bool        dlError(void)
{
    std::cerr << "Error : " << dlerror() << std::endl;
    return (false);
}

bool game(int argc, char **argv)
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

	IDisplay    *display = newDisplay("../externlib/commun/tileset1.png", 32, 30, 30, "lol");

	Grid<int> grid(30, 30);

	grid.fill(0);
	grid.setBorder(1);

	display->setBackground(grid);


	while (!display->exit())
	{
		display->update();
		display->drawTileGrid(0, 0, 0);
		display->drawTileGrid(1, 1, 1);
		display->drawTileGrid(2, 2, 2);
		display->drawTilePixel(12, 2*32+16, 2*32+16);
		display->render();
	}
	deleteDisplay(display);
	return (true);
}

int main(int argc, char **argv)
{
	Core menu;

	//menu.demo();
	menu.aState();
	return (0);
	menu.titleScreen();
	if (!(game(argc, argv)))
		return (0);
	menu.titleScreen();
}
