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
	WidgetLobby(Core &core, Snake const *snakes);
	~WidgetLobby(void);

	bool	addSnake(Snake const &snake, bool isYourSnake = false);
	void	addColor(std::string const &name, std::string const &pathColor);

	void	render(void);

private:
	std::vector< WidgetSnake * >				_snake;
	std::vector<sf::Texture>					_texture;
	std::vector<std::string>					_color;

	WidgetLobby &operator=(const WidgetLobby&);
	WidgetLobby(const WidgetLobby&);
};
