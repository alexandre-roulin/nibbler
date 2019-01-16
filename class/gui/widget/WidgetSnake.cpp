#include <Univers.hpp>
#include "WidgetSnake.hpp"
#include <gui/Core.hpp>

WidgetSnake::WidgetSnake(Core &core,
						 Snake const &snake,
						 std::vector<sf::Texture> &texture,
						 std::vector<std::string> &color,
						 bool isYourSnake = false) :
		AWidget(core),
		_texture(texture),
		_color(color),
		_snake(snake),
		_isYourSnake(isYourSnake) {
}

WidgetSnake::~WidgetSnake(void) {}


void			WidgetSnake::render(void)
{
	if (_snake.id == -1)
		return ;
	if (_isYourSnake)
		_renderYourSnake();
	else
		_renderOtherSnake();
}

void WidgetSnake::_renderOtherSnake(void) {
	unsigned int sizeTexture;


	ImGui::Begin(std::string(std::to_string(_snake.id) + std::string(_snake.name)).c_str(), NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);

	if (ImGui::GetWindowSize().x <
		ImGui::GetWindowSize().y - ImGui::GetFrameHeightWithSpacing() * 3)
		sizeTexture = ImGui::GetWindowSize().x * 0.8;
	else
		sizeTexture = ImGui::GetWindowSize().y * 0.8 -
					  ImGui::GetFrameHeightWithSpacing() * 3;

	ImGui::PushItemWidth(sizeTexture);
	ImGui::SetCursorPosX(4);
	ImGui::LabelText(_snake.name, "Name : ");


	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture) / 2);

	ImGui::Image(_texture[static_cast<int>(_snake.sprite)], sf::Vector2f(sizeTexture, sizeTexture));


	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture) / 2);
	if (!_snake.isReady)
	{
		Core::beginColor(Core::HUE_RED);
		if (ImGui::Button("Ready ?", sf::Vector2f(sizeTexture, ImGui::GetFrameHeight())) && _core.univers.getGameNetwork())
			_core.univers.getGameNetwork()->change_state_ready();;
	}
	else
	{
		Core::beginColor(Core::HUE_GREEN);
		if (ImGui::Button("Ready !", sf::Vector2f(sizeTexture, ImGui::GetFrameHeight())) && _core.univers.getGameNetwork())
			_core.univers.getGameNetwork()->change_state_ready();
	}
	Core::endColor();

	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture) / 2);
	if (ImGui::BeginCombo("", _color[static_cast<int>(_snake.sprite)].c_str(), ImGuiComboFlags_NoArrowButton))
	{
		unsigned int i = 0;
		for (auto const &e : _color)
		{
			if (ImGui::Selectable(e.c_str(), i == static_cast<int>(_snake.sprite)) && _core.univers.getGameNetwork())
				_core.univers.getGameNetwork()->change_sprite( static_cast<eSprite>(i));
			i++;
		}

		ImGui::EndCombo();
	}

	ImGui::End();
}

void WidgetSnake::_renderYourSnake(void) {
	unsigned int sizeTexture;
	unsigned int debug = 0;
	ImGui::Begin(std::string(std::to_string(_snake.id) + std::string(_snake.name)).c_str(), NULL, ImGuiWindowFlags_NoDecoration);

	if (ImGui::GetWindowSize().x <
		ImGui::GetWindowSize().y - ImGui::GetFrameHeightWithSpacing() * 3)
		sizeTexture = ImGui::GetWindowSize().x * 0.8;
	else
		sizeTexture = ImGui::GetWindowSize().y * 0.8 -
					  ImGui::GetFrameHeightWithSpacing() * 3;


	ImGui::PushItemWidth(sizeTexture);
	ImGui::SetCursorPosX(4);
	ImGui::LabelText(_snake.name, "Name : ");


	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture) / 2);

	ImGui::PushID(0);

	if (!_snake.isReady)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.9f, 0.9f));
		if (ImGui::Button("Ready ?", sf::Vector2f(sizeTexture, ImGui::GetFrameHeight())) && _core.univers.getGameNetwork())
			_core.univers.getGameNetwork()->change_state_ready();
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.33f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.33f, 0.8f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.33f, 0.9f, 0.9f));
		if (ImGui::Button("Ready !", sf::Vector2f(sizeTexture, ImGui::GetFrameHeight())) && _core.univers.getGameNetwork())
			_core.univers.getGameNetwork()->change_state_ready();
	}
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture) / 2);
	std::cout << "_renderYourSnake HERE"<< std::endl; // <<<<<<<<<< TODO <<<<<<<<<<<<
//_color[static_cast<int>(_snake.sprite) - static_cast<unsigned int>(eSprite::GREEN)].c_str()
	if (ImGui::BeginCombo("", _color[static_cast<int>(_snake.sprite) - static_cast<unsigned int>(eSprite::GREEN)].c_str(), ImGuiComboFlags_None))
	{
		std::cout << "_renderYourSnake " <<  std::endl; // <<<<<<<<<< TODO <<<<<<<<<<<<
		int i = static_cast<int>(eSprite::GREEN);
		std::cout << "_renderYourSnake " <<  std::endl; // <<<<<<<<<< TODO <<<<<<<<<<<<
		for (auto const &e : _color)
		{
			std::cout << "_renderYourSnake " <<  std::endl; // <<<<<<<<<< TODO <<<<<<<<<<<<
			if (ImGui::Selectable(e.c_str(), i == static_cast<int>(_snake.sprite)) && _core.univers.getGameNetwork()) {
				std::cout << "_renderYourSnake " << ++i << std::endl; // <<<<<<<<<< TODO <<<<<<<<<<<<
				_core.univers.getGameNetwork()->change_sprite(static_cast<eSprite>(i));
			}
			i++;
		}
		std::cout << "_renderYourSnake " <<  std::endl; // <<<<<<<<<< TODO <<<<<<<<<<<<
		ImGui::EndCombo();
	}
	std::cout << "_renderYourSnake " <<  std::endl;


	ImGui::End();
}
