#include "WidgetSnake.hpp"
#include <iostream>
WidgetSnake::WidgetSnake(void) :
AWidget()
{
	this->_color.emplace_back("Normal");
	this->_texture.emplace_back();
	if (!(this->_texture.back().loadFromFile("ecran_titre.png")))
		throw(AWidget::Constructor(std::string("WidgetSnake: Cant load [") + "ecran_titre.png" + "]"));
	this->_color.emplace_back("Normal 2 LOL");
	this->_texture.emplace_back();
	if (!(this->_texture.back().loadFromFile("ecran_titre.png")))
		throw(AWidget::Constructor(std::string("WidgetSnake: Cant load [") + "ecran_titre.png" + "]"));
}

WidgetSnake::~WidgetSnake(void)
{}

void			WidgetSnake::render(void)
{
	unsigned int		sizeTexture;

	ImGui::Begin("Snake", NULL, 0);

	if (ImGui::GetWindowSize().x < ImGui::GetWindowSize().y * 0.8)
		sizeTexture = ImGui::GetWindowSize().x * 0.8;
	else
		sizeTexture = ImGui::GetWindowSize().y * 0.8 * 0.8;

	ImGui::PushItemWidth((ImGui::GetWindowSize().x - sizeTexture) / 2);
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture) / 2);
	ImGui::Image(this->_texture.back(), sf::Vector2f(sizeTexture, sizeTexture));
	ImGui::End();
}
