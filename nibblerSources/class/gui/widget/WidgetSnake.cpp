#include <cores/Univers.hpp>
#include "WidgetSnake.hpp"
#include <gui/Gui.hpp>

WidgetSnake::WidgetSnake(Gui &core,
						 Snake const &snake,
						 std::map<eSprite, SpriteColorProperties> const &mapSprite,
						 bool isYourSnake) :
		AWidget(core),
		snake_(snake),
		mapSprite_(mapSprite),
		isYourSnake_(isYourSnake),
		sizeTexture_(0) {
	updateSizeTexture_();
}

void WidgetSnake::render(void) {
	if (!snake_.isValid)
		return;
	if (isYourSnake_)
		renderYourSnake_();
	else if (snake_.isIA)
		renderIa_();
	else
		renderOtherSnake_();
}

void WidgetSnake::updateSizeTexture_(void) {
	if (ImGui::GetWindowSize().x <
		ImGui::GetWindowSize().y - ImGui::GetFrameHeightWithSpacing() * 3)
		sizeTexture_ = ImGui::GetWindowSize().x * 0.8;
	else
		sizeTexture_ = ImGui::GetWindowSize().y * 0.8 - ImGui::GetFrameHeightWithSpacing() * 3;
}

void WidgetSnake::renderName_() const {
	ImGui::PushItemWidth(sizeTexture_);
	ImGui::SetCursorPosX(4);
	ImGui::LabelText(snake_.name, "Name : ");
}

void WidgetSnake::renderImage_() const {
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture_) / 2);
	ImGui::Image(mapSprite_.at(snake_.sprite).sprite, sf::Vector2f(sizeTexture_, sizeTexture_));
}

void WidgetSnake::renderStaticDataSnake_() const {
	renderName_();
	renderImage_();
}

void WidgetSnake::renderSelectionColor_() const {

	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	int flagImGuiCombo;

	if (isYourSnake_)
		flagImGuiCombo = ImGuiComboFlags_None;
	else
		flagImGuiCombo = ImGuiComboFlags_NoArrowButton;
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture_) / 2);
	if (ImGui::BeginCombo("", mapSprite_.at(snake_.sprite).name.c_str(), flagImGuiCombo)) {
		for (auto const &e : mapSprite_) {
			if (ImGui::Selectable(e.second.name.c_str(), e.first == snake_.sprite) && ptr)
				ptr->changeSprite(e.first);
		}
		ImGui::EndCombo();
	}
}


void WidgetSnake::renderIa_(void) {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	if (core_.univers.isServer())
		ImGui::Begin(std::string(std::to_string(snake_.id) + std::string(snake_.name)).c_str(), NULL,
					 ImGuiWindowFlags_NoDecoration);
	else
		ImGui::Begin(std::string(std::to_string(snake_.id) + std::string(snake_.name)).c_str(), NULL,
					 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);

	updateSizeTexture_();

	renderStaticDataSnake_();

	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture_) / 2);

	Gui::beginColor(Gui::HUE_PURPLE);
	if (core_.univers.isServer()) {
		if (ptr && ImGui::Button("Delete", sf::Vector2f(sizeTexture_, ImGui::GetFrameHeight())))
			core_.univers.deleteBobby(snake_.id);
	}
	else
		ImGui::Button("Bot", sf::Vector2f(sizeTexture_, ImGui::GetFrameHeight()));

	Gui::endColor();
	ImGui::End();
}

void WidgetSnake::renderOtherSnake_(void) {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	ImGui::Begin(std::string(std::to_string(snake_.id) + std::string(snake_.name)).c_str(), NULL,
				 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);

	updateSizeTexture_();

	renderStaticDataSnake_();

	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture_) / 2);


	if (!snake_.isReady) {
		Gui::beginColor(Gui::HUE_RED);
		if (ImGui::Button("Ready ?", sf::Vector2f(sizeTexture_, ImGui::GetFrameHeight())) && ptr)
			ptr->changeStateReady(true);
	} else {
		Gui::beginColor(Gui::HUE_GREEN);
		if (ImGui::Button("Ready !", sf::Vector2f(sizeTexture_, ImGui::GetFrameHeight())) && ptr)
			ptr->changeStateReady(false);
	}
	Gui::endColor();

	renderSelectionColor_();
	ImGui::End();
}

void WidgetSnake::renderYourSnake_(void) {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	ImGui::Begin(std::string(std::to_string(snake_.id) + std::string(snake_.name)).c_str(), NULL,
				 ImGuiWindowFlags_NoDecoration);

	updateSizeTexture_();

	renderStaticDataSnake_();

	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture_) / 2);
	if (!snake_.isReady) {
		Gui::beginColor(Gui::HUE_RED);
		if (ImGui::Button("Ready ?", sf::Vector2f(sizeTexture_, ImGui::GetFrameHeight())) && ptr)
			ptr->changeStateReady(true);
	} else {
		Gui::beginColor(Gui::HUE_GREEN);
		if (ImGui::Button("Ready !", sf::Vector2f(sizeTexture_, ImGui::GetFrameHeight())) && ptr)
			ptr->changeStateReady(false);
	}
	Gui::endColor();

	renderSelectionColor_();
	ImGui::End();
}
