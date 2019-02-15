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
		isYourSnake_(isYourSnake) {
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

unsigned int WidgetSnake::sizeTexture_(void) const {
	if (ImGui::GetWindowSize().x <
		ImGui::GetWindowSize().y - ImGui::GetFrameHeightWithSpacing() * 3)
		return (ImGui::GetWindowSize().x * 0.8);
	else
		return (ImGui::GetWindowSize().y * 0.8 - ImGui::GetFrameHeightWithSpacing() * 3);
}

void WidgetSnake::renderName_(unsigned int sizeTexture) const {
	ImGui::PushItemWidth(sizeTexture);
	ImGui::SetCursorPosX(4);
	ImGui::LabelText(snake_.name, "Name : ");
}

void WidgetSnake::renderImage_(unsigned int sizeTexture) const {
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture) / 2);
	ImGui::Image(mapSprite_.at(snake_.sprite).sprite, sf::Vector2f(sizeTexture, sizeTexture));
}

void WidgetSnake::renderSelectionColor_(unsigned int sizeTexture) const {

	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	int flagImGuiCombo;

	if (isYourSnake_)
		flagImGuiCombo = ImGuiComboFlags_None;
	else
		flagImGuiCombo = ImGuiComboFlags_NoArrowButton;
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture) / 2);
	if (ImGui::BeginCombo("", mapSprite_.at(snake_.sprite).name.c_str(), flagImGuiCombo)) {
		for (auto const &e : mapSprite_) {
			if (ImGui::Selectable(e.second.name.c_str(), e.first == snake_.sprite) && ptr)
				ptr->changeSprite(e.first);
		}
		ImGui::EndCombo();
	}
}


void WidgetSnake::renderIa_(void) {
	unsigned int sizeTexture;

	if (core_.univers.isServer()) {
		ImGui::Begin(std::string(std::to_string(snake_.id) + std::string(snake_.name)).c_str(), NULL,
					 ImGuiWindowFlags_NoDecoration);
	} else {
		ImGui::Begin(std::string(std::to_string(snake_.id) + std::string(snake_.name)).c_str(), NULL,
					 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);
	}

	sizeTexture = sizeTexture_();
	renderName_(sizeTexture);
	renderImage_(sizeTexture);

	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture) / 2);
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	if (core_.univers.isServer()) {
		Gui::beginColor(Gui::HUE_PURPLE);
		if (ImGui::Button("Delete", sf::Vector2f(sizeTexture, ImGui::GetFrameHeight())) && ptr) {
			std::cout << "BOOB ?!! DO SOMETHING" << std::endl;
			core_.univers.delete_ia(snake_.id);
		}
	}

	Gui::endColor();

	//renderSelectionColor_(sizeTexture);
	ImGui::End();
}

void WidgetSnake::renderOtherSnake_(void) {
	unsigned int sizeTexture;

	ImGui::Begin(std::string(std::to_string(snake_.id) + std::string(snake_.name)).c_str(), NULL,
				 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);

	sizeTexture = sizeTexture_();
	renderName_(sizeTexture);
	renderImage_(sizeTexture);

	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture) / 2);

	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	if (!snake_.isReady) {
		Gui::beginColor(Gui::HUE_RED);
		if (ImGui::Button("Ready ?", sf::Vector2f(sizeTexture, ImGui::GetFrameHeight())) && ptr)
			ptr->changeStateReady(true);
	} else {
		Gui::beginColor(Gui::HUE_GREEN);
		if (ImGui::Button("Ready !", sf::Vector2f(sizeTexture, ImGui::GetFrameHeight())) && ptr)
			ptr->changeStateReady(false);
	}
	Gui::endColor();

	renderSelectionColor_(sizeTexture);
	ImGui::End();
}

void WidgetSnake::renderYourSnake_(void) {
	unsigned int sizeTexture;

	ImGui::Begin(std::string(std::to_string(snake_.id) + std::string(snake_.name)).c_str(), NULL,
				 ImGuiWindowFlags_NoDecoration);

	sizeTexture = sizeTexture_();
	renderName_(sizeTexture);
	renderImage_(sizeTexture);
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	ImGui::PushID(0);
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture) / 2);
	if (!snake_.isReady) {
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(0.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(0.0f, 0.8f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(0.0f, 0.9f, 0.9f));
		if (ImGui::Button("Ready ?", sf::Vector2f(sizeTexture, ImGui::GetFrameHeight())) && ptr)
			ptr->changeStateReady(true);
	} else {
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4) ImColor::HSV(0.33f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4) ImColor::HSV(0.33f, 0.8f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4) ImColor::HSV(0.33f, 0.9f, 0.9f));
		if (ImGui::Button("Ready !", sf::Vector2f(sizeTexture, ImGui::GetFrameHeight())) && ptr)
			ptr->changeStateReady(false);
	}
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	renderSelectionColor_(sizeTexture);
	ImGui::End();
}
