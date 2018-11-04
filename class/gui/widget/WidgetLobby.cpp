#include "WidgetLobby.hpp"
#include <gui/Core.hpp>

WidgetLobby::WidgetLobby(Core &core, Snake const *snakes) :
AWidget(),
_core(core)
{
	for (unsigned i = 0; i < MAX_SNAKE; i++)
		this->addSnake(snakes[i]);
	this->_color.emplace_back("snake_green");
	this->_texture.emplace_back();
	if (!(this->_texture.back().loadFromFile("ressource/snake_presentation/snake_green.png")))
		throw(AWidget::Constructor(std::string("WidgetLobby: Cant load [") + "snake_green.png" + "]"));
	this->_color.emplace_back("snake_purple");
	this->_texture.emplace_back();
	if (!(this->_texture.back().loadFromFile("ressource/snake_presentation/snake_purple.png")))
		throw(AWidget::Constructor(std::string("WidgetLobby: Cant load [") + "snake_purple.png" + "]"));
	this->_color.emplace_back("snake_blue");
	this->_texture.emplace_back();
	if (!(this->_texture.back().loadFromFile("ressource/snake_presentation/snake_blue.png")))
		throw(AWidget::Constructor(std::string("WidgetLobby: Cant load [") + "snake_blue.png" + "]"));
}

WidgetLobby::~WidgetLobby(void)
{
	for (auto &snake : this->_snake)
		delete snake;
}

bool			WidgetLobby::addSnake(Snake const &snake, bool isYourSnake)
{
	if (this->_snake.size() < MAX_SNAKE)
	{
		this->_snake.emplace_back(new WidgetSnake(this->_core, snake, this->_texture, this->_color, isYourSnake));
		return (true);
	}
	return (false);
}

void			WidgetLobby::render(void)
{
	for (unsigned int i = 0; i < this->_snake.size(); i++)
	{
		sf::Vector2<unsigned int> ab = sf::Vector2<unsigned int>((100 / (MAX_SNAKE / 2)) * (i % 4), 25 * ((i && i <= 4)));
		ImGui::SetNextWindowPos(this->_core.positionByPercent(ab));
		ImGui::SetNextWindowSize(this->_core.positionByPercent(sf::Vector2<unsigned int>(100 / (MAX_SNAKE / 2), 50 / 2)));
		this->_snake[i]->render();
	}
}
