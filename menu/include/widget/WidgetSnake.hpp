#pragma once
#include "IWidget.hpp"
#include <string>
#include <vector>
#include <list>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>
#include "WidgetChat.hpp"

class WidgetSnake : public IWidget {
public:
	WidgetSnake(void);
	WidgetSnake(void (*callback)(void *), void *ptr);
	~WidgetSnake(void);


	void	render(void);

private:
	bool										_active;
	std::vector<sf::Texture>					_texture;
	std::vector<std::string>					_color;
	unsigned int								_indexColor;

	WidgetSnake &operator=(const WidgetSnake&);
	WidgetSnake(const WidgetSnake&);
};
