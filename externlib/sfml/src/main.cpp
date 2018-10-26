#include "DisplaySfml.hpp"

int         main(int argc, char **argv)
{
    static_cast<void>(argc);
    static_cast<void>(argv);
    DisplaySfml sfml("../sdl/tileset1.png", 32, 30, 30, "lol");

    while (!sfml.exit())
    {
        sfml.update();
        sfml.drawTile(0, 0, 0);
        sfml.drawTile(1, 1, 1);
        sfml.drawTile(2, 2, 2);
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
