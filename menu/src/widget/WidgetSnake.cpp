#include "WidgetSnake.hpp"
#include <iostream>
WidgetSnake::WidgetSnake(std::string const &str) :
AWidget(),
_name(str),
_indexColor(0),
_isReady(false)
{
	this->_color.emplace_back("snake_1");
	this->_texture.emplace_back();
	if (!(this->_texture.back().loadFromFile("snake_1.png")))
		throw(AWidget::Constructor(std::string("WidgetSnake: Cant load [") + "snake_1.png" + "]"));
	this->_color.emplace_back("snake_2");
	this->_texture.emplace_back();
	if (!(this->_texture.back().loadFromFile("snake_2.png")))
		throw(AWidget::Constructor(std::string("WidgetSnake: Cant load [") + "snake_2.png" + "]"));
}

WidgetSnake::~WidgetSnake(void)
{}

void			WidgetSnake::render(void)
{
	unsigned int		sizeTexture;

	ImGui::Begin(this->_name.c_str(), NULL, 0);

	if (ImGui::GetWindowSize().x < ImGui::GetWindowSize().y - ImGui::GetFrameHeightWithSpacing() * 3)
		sizeTexture = ImGui::GetWindowSize().x * 0.8;
	else
		sizeTexture = ImGui::GetWindowSize().y * 0.8 - ImGui::GetFrameHeightWithSpacing() * 3;



	ImGui::PushItemWidth(sizeTexture);
	ImGui::SetCursorPosX(4);
	ImGui::LabelText(this->_name.c_str(), "Name : ");


	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture) / 2);
	ImGui::Image(this->_texture[this->_indexColor], sf::Vector2f(sizeTexture, sizeTexture));


	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture) / 2);
	ImGui::PushID(0);
	if (!this->_isReady)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.9f, 0.9f));
		if (ImGui::Button("Ready ?", sf::Vector2f(sizeTexture, ImGui::GetFrameHeight())))
			this->_isReady = true;
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.33f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.33f, 0.8f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.33f, 0.9f, 0.9f));
		if (ImGui::Button("Ready !", sf::Vector2f(sizeTexture, ImGui::GetFrameHeight())))
			this->_isReady = false;
	}
	ImGui::PopStyleColor(3);
	ImGui::PopID();

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
