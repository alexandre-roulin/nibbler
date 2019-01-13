#include "WidgetLobby.hpp"
#include <Univers.hpp>
#include <gui/Core.hpp>

WidgetLobby::WidgetLobby(Core &core, Snake const *snakes) :
AWidget(core)
{
	for (unsigned i = 0; i < MAX_SNAKE; i++)
	{
		std::cout << snakes[i].id << "  |  isYour ? i[" << i <<  "] id[" << this->_core.univers.getGameNetwork()->getId() << "] = [" << (i ==
				this->_core.univers.getGameNetwork()->getId()) << "]" << std::endl;
		this->addSnake(snakes[i], (i == this->_core.univers.getGameNetwork()->getId()));
	}
	this->addColor("Green", "ressource/snake_presentation/snake_green.png");
	this->addColor("Blue", "ressource/snake_presentation/snake_blue.png");
	this->addColor("Purple", "ressource/snake_presentation/snake_purple.png");
	this->addColor("Pink", "ressource/snake_presentation/snake_pink.png");
	this->addColor("Grey", "ressource/snake_presentation/snake_grey.png");
	this->addColor("Yellow", "ressource/snake_presentation/snake_yellow.png");
	this->addColor("Orange", "ressource/snake_presentation/snake_orange.png");
	this->addColor("Red", "ressource/snake_presentation/snake_red.png");
}

WidgetLobby::~WidgetLobby(void)
{
	for (auto &snake : this->_snake)
		delete snake;
}

void			WidgetLobby::addColor(std::string const &name, std::string const &pathColor) {
	this->_color.emplace_back(name);
	this->_texture.emplace_back();
	if (!(this->_texture.back().loadFromFile(pathColor)))
		throw(AWidget::Constructor(std::string("WidgetLobby: Cant load [") + pathColor + "]"));
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
