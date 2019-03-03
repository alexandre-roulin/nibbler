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

	void addSnake(Snake const &snake, WidgetSnake::SnakeType type);

	void addColor(eSprite color, std::string const &name, std::string const &pathColor);

private:
	std::map<eSprite, SpriteColorProperties> mapSprite_;
	std::deque<WidgetSnake> snakeWidget_;
	std::weak_ptr<SnakeArrayContainer> snakes_;

	bool reload_();
	void update_() override;
	void beginContent_() override;
};
