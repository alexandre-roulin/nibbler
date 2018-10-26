#include "DisplaySfml.hpp"

int         main(int argc, char **argv)
{
    static_cast<void>(argc);
    static_cast<void>(argv);
    DisplaySfml sfml("../sdl/tileset1.png", 32, 30, 30, "lol");

    Grid<int> grid(30, 30);

	grid.fill(0);
	grid.setBorder(1);

	sfml.setBackground(grid);

    while (!sfml.exit())
    {
        sfml.update();
        sfml.drawTileGrid(0, 0, 0);
        sfml.drawTileGrid(1, 1, 1);
        sfml.drawTileGrid(2, 2, 2);
        sfml.drawTilePixel(12, 2*32+16, 2*32+16);
        sfml.render();
    }
/*
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
    */
    return (0);
}
