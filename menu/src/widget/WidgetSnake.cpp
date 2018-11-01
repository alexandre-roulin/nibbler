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

	if (ImGui::GetWindowSize().x < ImGui::GetWindowSize().y - ImGui::GetFrameHeightWithSpacing())
		sizeTexture = ImGui::GetWindowSize().x * 0.8;
	else
		sizeTexture = ImGui::GetWindowSize().y * 0.8 - ImGui::GetFrameHeightWithSpacing();

	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture) / 2);
	ImGui::Image(this->_texture.back(), sf::Vector2f(sizeTexture, sizeTexture));

	ImGui::PushItemWidth(sizeTexture);
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture) / 2);

	if (ImGui::BeginCombo("", this->_color[this->_indexColor].c_str(), ImGuiComboFlags_NoArrowButton))
	{
		unsigned int i = 0;
		for (auto const &e : this->_color)
		{
			if (ImGui::Selectable(e.c_str(), i == this->_indexColor))
				this->_indexColor = i;
			i++;
		}

		ImGui::EndCombo();
	}

	ImGui::End();
}
