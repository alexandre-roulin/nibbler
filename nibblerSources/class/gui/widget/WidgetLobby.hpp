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
	WidgetLobby(Core &core);
	~WidgetLobby(void);

	void	addSnake(Snake const &snake, bool isYourSnake = false);
	void	addColor(std::string const &name, std::string const &pathColor);

	void	render(void);

private:

	void		_reload();

	std::vector< WidgetSnake * >				_snake;
	std::vector<sf::Texture>					_texture;
	std::vector<std::string>					_color;
	std::array<Snake, MAX_SNAKE>		const  *snakes_;

	WidgetLobby &operator=(const WidgetLobby&);
	WidgetLobby(const WidgetLobby&);
};
