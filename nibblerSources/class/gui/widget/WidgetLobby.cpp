#include "WidgetLobby.hpp"
#include <Univers.hpp>
#include <gui/Core.hpp>
#include <network/SnakeClient.hpp>

WidgetLobby::WidgetLobby(Core &core) :
AWidget(core),
snakes_(nullptr)
{
	_reload();

	addColor("Green", (_core.getPathRessources() / "snake_presentation/snake_green.png").generic_string());
	addColor("Blue", (_core.getPathRessources() / "snake_presentation/snake_blue.png").generic_string());
	addColor("Purple", (_core.getPathRessources() / "snake_presentation/snake_purple.png").generic_string());
	addColor("Pink", (_core.getPathRessources() / "snake_presentation/snake_pink.png").generic_string());
	addColor("Grey", (_core.getPathRessources() / "snake_presentation/snake_grey.png").generic_string());
	addColor("Yellow", (_core.getPathRessources() / "snake_presentation/snake_yellow.png").generic_string());
	addColor("Orange", (_core.getPathRessources() / "snake_presentation/snake_orange.png").generic_string());
	addColor("Red", (_core.getPathRessources() / "snake_presentation/snake_red.png").generic_string());
}

WidgetLobby::~WidgetLobby(void)
{
	for (auto &snake : _snake)
		delete snake;
}

void			WidgetLobby::addColor(std::string const &name, std::string const &pathColor) {
	_color.emplace_back(name);
	_texture.emplace_back();
	if (!(_texture.back().loadFromFile(pathColor)))
		throw(AWidget::Constructor(std::string("WidgetLobby: Cant load [") + pathColor + "]"));
}


void			WidgetLobby::addSnake(Snake const &snake, bool isYourSnake)
{
	if (_snake.size() < MAX_SNAKE)
		_snake.emplace_back(new WidgetSnake(_core, snake, _texture, _color, isYourSnake));
}

void			WidgetLobby::_reload()
{
	if (_core.univers.getSnakeClient()) {
		snakes_ = &_core.univers.getSnakeClient()->getSnakeArray_();
		for (unsigned i = 0; i < MAX_SNAKE; i++) {
//			std::cout << snakes[i].id << "  |  isYour ? i[" << i <<  "] id[" << _core.univers.getSnakeClient()->getId_() << "] = [" << (i ==
//			_core.univers.getSnakeClient()->getId_()) << "]" << std::endl;
			addSnake((*snakes_)[i], (i == _core.univers.getSnakeClient()->getId_()));
		}
	}
}

void			WidgetLobby::render(void)
{
	if (_core.univers.getSnakeClient()) {
		_reload();
		for (unsigned int i = 0; i < _snake.size(); i++)
		{
			sf::Vector2<unsigned int> ab = sf::Vector2<unsigned int>((100 / (MAX_SNAKE / 2)) * (i % 4), 25 * ((i && i <= 4)));
			ImGui::SetNextWindowPos(_core.positionByPercent(ab));
			ImGui::SetNextWindowSize(_core.positionByPercent(sf::Vector2<unsigned int>(100 / (MAX_SNAKE / 2), 50 / 2)));
			_snake[i]->render();
		}
	}
}
