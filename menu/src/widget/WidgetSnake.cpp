#include "WidgetSnake.hpp"

WidgetSnake::WidgetSnake(void) :
_active(true)
{
	this->_color.emplace_back("Normal");
	this->_texture.emplace_back();
	this->_texture.back().loadFromFile("ecran_titre.png");
	this->_color.emplace_back("Normal 2 LOL");
	this->_texture.emplace_back();
	this->_texture.back().loadFromFile("ecran_titre.png");
}

WidgetSnake::~WidgetSnake(void)
{}

void			WidgetSnake::render(void)
{
	ImGui::Begin("Exit", NULL, 0);
	ImGui::Image(this->_texture.back(), sf::Vector2f(50, 50));
	ImGui::End();
}
