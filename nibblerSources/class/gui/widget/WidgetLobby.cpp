#include "WidgetLobby.hpp"
#include <Univers.hpp>
#include <gui/Core.hpp>
#include "Vector2D.tpp"
#include <math.h>
#include <network/SnakeClient.hpp>

WidgetLobby::WidgetLobby(Core &core) :
AWidget(core)
{
	addColor("Green", (_core.getPathRessources() / "snake_presentation/snake_green.png").generic_string());
	addColor("Blue", (_core.getPathRessources() / "snake_presentation/snake_blue.png").generic_string());
	addColor("Purple", (_core.getPathRessources() / "snake_presentation/snake_purple.png").generic_string());
	addColor("Pink", (_core.getPathRessources() / "snake_presentation/snake_pink.png").generic_string());
	addColor("Grey", (_core.getPathRessources() / "snake_presentation/snake_grey.png").generic_string());
	addColor("Yellow", (_core.getPathRessources() / "snake_presentation/snake_yellow.png").generic_string());
	addColor("Orange", (_core.getPathRessources() / "snake_presentation/snake_orange.png").generic_string());
	addColor("Red", (_core.getPathRessources() / "snake_presentation/snake_red.png").generic_string());
}

WidgetLobby::~WidgetLobby(void) = default;

void			WidgetLobby::addColor(std::string const &name, std::string const &pathColor) {
	_color.emplace_back(name);
	_texture.emplace_back();
	if (!(_texture.back().loadFromFile(pathColor)))
		throw(AWidget::Constructor(std::string("WidgetLobby: Cant load [") + pathColor + "]"));
}


void			WidgetLobby::addSnake(Snake const &snake, bool isYourSnake) {
	if (snakeWidget_.size() < SNAKE_MAX)
		snakeWidget_.emplace_back(std::make_unique<WidgetSnake>(_core, snake, _texture, _color, isYourSnake));
}

void			WidgetLobby::_reload() {
	snakeWidget_.clear();
	snakeWidget_.reserve(8);
	for (unsigned int i = 0; i < SNAKE_MAX; i++) {
		if (_core.univers.getSnakeClient())
			addSnake(snakes_[i], (i == _core.univers.getSnakeClient()->getId_()));
		else
			addSnake(snakes_[i], false);
	}
}


void			WidgetLobby::render(void) {
	snakes_ = _core.univers.getSnakeArray_();
	sf::Vector2<unsigned int> placesForSnakes;
	sf::Vector2<unsigned int> percentPlaceOfSnake;

	size_t i = std::count_if(snakes_.begin(), snakes_.end(), [](Snake const & s) {return s.id != -1;});
	placesForSnakes.x = 4;
	placesForSnakes.y = floor(i / 4) + 1;
	_reload();
	for (i = 0; i < snakeWidget_.size() && snakes_[i].id != -1; i++) {
		percentPlaceOfSnake.x = ((100 / placesForSnakes.x) * (i % placesForSnakes.x));
		percentPlaceOfSnake.y = ((50 / placesForSnakes.y) * (i / placesForSnakes.x));
		ImGui::SetNextWindowPos(_core.positionByPercent(percentPlaceOfSnake));
		ImGui::SetNextWindowSize(_core.positionByPercent(
				sf::Vector2<unsigned int>(100 / placesForSnakes.x, 50 / placesForSnakes.y)));
		snakeWidget_[i]->render();
	}
}
