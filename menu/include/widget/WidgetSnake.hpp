#pragma once
#include "AWidget.hpp"
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

class WidgetSnake : public AWidget {
public:
	WidgetSnake(std::string const &, std::vector<sf::Texture> &, std::vector<std::string> &, bool);
	~WidgetSnake(void);


	void	render(void);

private:

	std::vector<sf::Texture>					&_texture;
	std::vector<std::string>					&_color;
	std::string									_name;
	unsigned int								_indexColor;
	bool										_isReady;
	bool										_isYourSnake;

	void		_renderYourSnake(void);

	WidgetSnake &operator=(const WidgetSnake&);
	WidgetSnake(const WidgetSnake&);
	WidgetSnake(void);
};
