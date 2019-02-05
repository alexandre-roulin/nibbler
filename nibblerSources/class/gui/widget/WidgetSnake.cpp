#include <Univers.hpp>
#include "WidgetSnake.hpp"
#include <gui/Core.hpp>
#include <network/SnakeClient.hpp>

WidgetSnake::WidgetSnake(Core &core,
						 Snake const &snake,
						 std::vector<sf::Texture> &texture,
						 std::vector<std::string> &color,
						 bool isYourSnake = false) :
		AWidget(core),
		_snake(snake),
		_texture(texture),
		_color(color),
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

unsigned int WidgetSnake::_sizeTexture(void) const {
	if (ImGui::GetWindowSize().x <
		ImGui::GetWindowSize().y - ImGui::GetFrameHeightWithSpacing() * 3)
		return (ImGui::GetWindowSize().x * 0.8);
	else
		return (ImGui::GetWindowSize().y * 0.8 - ImGui::GetFrameHeightWithSpacing() * 3);
}

void WidgetSnake::_renderName(unsigned int sizeTexture) const {
	ImGui::PushItemWidth(sizeTexture);
	ImGui::SetCursorPosX(4);
	ImGui::LabelText(_snake.name, "Name : ");
}

void WidgetSnake::_renderImage(unsigned int sizeTexture) const {
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture) / 2);
	ImGui::Image(_texture[static_cast<int>(_snake.sprite) - static_cast<unsigned int>(eSprite::GREEN)], sf::Vector2f(sizeTexture, sizeTexture));
}

void WidgetSnake::_renderSelectionColor(unsigned int sizeTexture) const {

	int flagImGuiCombo;

	if (_isYourSnake)
		flagImGuiCombo = ImGuiComboFlags_None;
	else
		flagImGuiCombo = ImGuiComboFlags_NoArrowButton;
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture) / 2);
	if (ImGui::BeginCombo("", _color[static_cast<int>(_snake.sprite) - static_cast<unsigned int>(eSprite::GREEN)].c_str(), flagImGuiCombo))
	{
		int i = static_cast<int>(eSprite::GREEN);
		for (auto const &e : _color)
		{
			if (ImGui::Selectable(e.c_str(), i == static_cast<int>(_snake.sprite)) && _core.univers.getSnakeClient())
				_core.univers.getSnakeClient()->changeSprite(static_cast<eSprite>(i));
			i++;
		}
		ImGui::EndCombo();
	}
}


void WidgetSnake::_renderOtherSnake(void) {
	unsigned int sizeTexture;

	ImGui::Begin(std::string(std::to_string(_snake.id) + std::string(_snake.name)).c_str(), NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);

	sizeTexture = _sizeTexture();
	_renderName(sizeTexture);
	_renderImage(sizeTexture);

	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture) / 2);
	if (!_snake.isReady)
	{
		Core::beginColor(Core::HUE_RED);
		if (ImGui::Button("Ready ?", sf::Vector2f(sizeTexture, ImGui::GetFrameHeight())) && _core.univers.getSnakeClient())
			_core.univers.getSnakeClient()->changeStateReady(true);
	}
	else
	{
		Core::beginColor(Core::HUE_GREEN);
		if (ImGui::Button("Ready !", sf::Vector2f(sizeTexture, ImGui::GetFrameHeight())) && _core.univers.getSnakeClient())
			_core.univers.getSnakeClient()->changeStateReady(false);
	}
	Core::endColor();

	_renderSelectionColor(sizeTexture);
	ImGui::End();
}

void WidgetSnake::_renderYourSnake(void) {
	unsigned int sizeTexture;

	ImGui::Begin(std::string(std::to_string(_snake.id) + std::string(_snake.name)).c_str(), NULL, ImGuiWindowFlags_NoDecoration);

	sizeTexture = _sizeTexture();
	_renderName(sizeTexture);
	_renderImage(sizeTexture);

	ImGui::PushID(0);
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture) / 2);
	if (!_snake.isReady)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.9f, 0.9f));
		if (ImGui::Button("Ready ?", sf::Vector2f(sizeTexture, ImGui::GetFrameHeight())) && _core.univers.getSnakeClient())
			_core.univers.getSnakeClient()->changeStateReady(true);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.33f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.33f, 0.8f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.33f, 0.9f, 0.9f));
		if (ImGui::Button("Ready !", sf::Vector2f(sizeTexture, ImGui::GetFrameHeight())) && _core.univers.getSnakeClient())
			_core.univers.getSnakeClient()->changeStateReady(false);
	}
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	_renderSelectionColor(sizeTexture);
	ImGui::End();
}
