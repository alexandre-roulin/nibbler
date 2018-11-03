#pragma once
#include "AWidget.hpp"
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "WidgetSnake.hpp"
#include "nibbler.hpp"

class Core;

class WidgetLobby : public AWidget {
public:
	WidgetLobby(Core const &core, Snake const *snakes);
	~WidgetLobby(void);

	bool	addSnake(Snake const &snake, bool isYourSnake = false);

	void	render(void);

private:
	Core const									&_core;
	std::vector< WidgetSnake * >				_snake;
	std::vector<sf::Texture>					_texture;
	std::vector<std::string>					_color;

	WidgetLobby &operator=(const WidgetLobby&);
	WidgetLobby(const WidgetLobby&);
};
