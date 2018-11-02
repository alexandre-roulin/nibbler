#include "WidgetLobby.hpp"
#include "Core.hpp"

WidgetLobby::WidgetLobby(Core const &core) :
AWidget(),
_core(core)
{
	this->_color.emplace_back("snake_1");
	this->_texture.emplace_back();
	if (!(this->_texture.back().loadFromFile("snake_1.png")))
		throw(AWidget::Constructor(std::string("WidgetLobby: Cant load [") + "snake_1.png" + "]"));
	this->_color.emplace_back("snake_2");
	this->_texture.emplace_back();
	if (!(this->_texture.back().loadFromFile("snake_2.png")))
		throw(AWidget::Constructor(std::string("WidgetLobby: Cant load [") + "snake_2.png" + "]"));
}

WidgetLobby::~WidgetLobby(void)
{
	for (auto &snake : this->_snake)
		delete snake;
}

void			WidgetLobby::addSnake(std::string const &name)
{
	this->_snake.emplace_back(new WidgetSnake(name, this->_texture, this->_color));
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
