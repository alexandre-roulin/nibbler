#pragma once
#include "AWidget.hpp"
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "nibbler.hpp"

class Core;

class WidgetSnake : public AWidget {
public:
	WidgetSnake(Core &, Snake const &, std::vector<sf::Texture> &, std::vector<std::string> &, bool);
	~WidgetSnake(void);


	void	render(void);

private:
	Core										&_core;
	Snake const									&_snake;
	std::vector<sf::Texture>					&_texture;
	std::vector<std::string>					&_color;
	bool										_isYourSnake;

	void		_renderYourSnake(void);
	void		_renderOtherSnake(void);

	WidgetSnake &operator=(const WidgetSnake&);
	WidgetSnake(const WidgetSnake&);
	WidgetSnake(void);
};
