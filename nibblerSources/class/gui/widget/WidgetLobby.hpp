#pragma once

#include "AWidget.hpp"
#include "SpriteColorProperties.hpp"
#include <vector>
#include <array>
#include <deque>
#include "WidgetSnake.hpp"
#include "nibbler.hpp"
#include <cores/Snake.hpp>

#define WIDGET_LOBBY_DIRECTORY_SNAKE_PRESENTATION "snake_presentation"

class Gui;

class WidgetLobby : public AWidget {
public:
	WidgetLobby(Gui &core);
	~WidgetLobby() override = default;
	WidgetLobby() = delete;
	WidgetLobby &operator=(const WidgetLobby &) = default;
	WidgetLobby(const WidgetLobby &) = default;

	void addSnake(SnakeUI const &snake, WidgetSnake::SnakeType type);

	void addColor(eSprite color, std::string const &name, std::string const &pathColor);

	void render(void) override;

private:
	std::map<eSprite, SpriteColorProperties> mapSprite_;
	std::deque<WidgetSnake> snakeWidget_;
	SnakeUIArrayContainer snakes_;

	void _reload();
};
