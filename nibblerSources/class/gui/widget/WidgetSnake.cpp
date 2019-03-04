#include <cores/Univers.hpp>
#include "WidgetSnake.hpp"
#include <gui/Gui.hpp>

WidgetSnake::WidgetSnake(Gui &core,
						 Snake const &snake,
						 std::map<eSprite, SpriteColorProperties> const &mapSprite,
						 SnakeType type) :
		AWidget(core, "", 0),
		snake_(snake),
		mapSprite_(mapSprite),
		type_(type),
		sizeTexture_(0) {
	updateSizeTexture_();
}

void WidgetSnake::update_() {
	if (!snake_.isValid || !snake_.isReadyToExpose) {
		active_ = false;
		return ;
	}
	winName_ = std::string(std::to_string(snake_.id) + std::string(snake_.name)).c_str();
	if (type_ == kYour)
		winFlags_ = ImGuiWindowFlags_NoDecoration;
	else if (type_ == kIa) {
		if (core_.univers.isServer())
			winFlags_ = ImGuiWindowFlags_NoDecoration;
		else
			winFlags_ = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs;
	}
	else
		winFlags_ = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs;
	active_ = true;
}

void WidgetSnake::beginContent_() {
	if (!snake_.isValid || !snake_.isReadyToExpose)
		return;
	if (type_ == kYour)
		renderYourSnake_();
	else if (type_ == kIa)
		renderIa_();
	else
		renderOtherSnake_();
}

void WidgetSnake::updateSizeTexture_() {
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

void WidgetSnake::renderSelectionColor_(SnakeClient::boost_shared_ptr &ptr) const {

	int flagImGuiCombo;

	if (type_ == kYour || type_ == kIa)
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


void WidgetSnake::renderIa_() {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());
	SnakeClient::boost_shared_ptr ptrBobby(core_.univers.getBobbyClient(snake_.id).lock());

	updateSizeTexture_();
	renderStaticDataSnake_();

	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture_) / 2);

	Gui::beginColor(Gui::Gui::eColor::kPurple);
	if (core_.univers.isServer()) {
		if (ptr && ptrBobby && ImGui::Button("Delete", sf::Vector2f(sizeTexture_, ImGui::GetFrameHeight())))
			core_.univers.deleteBobby(ptrBobby);
	}
	else
		ImGui::Button("Bot", sf::Vector2f(sizeTexture_, ImGui::GetFrameHeight()));

	Gui::endColor();

	if (core_.univers.isServer() && ptrBobby)
		renderSelectionColor_(ptrBobby);
}

void WidgetSnake::renderOtherSnake_() {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	updateSizeTexture_();
	renderStaticDataSnake_();

	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture_) / 2);

	if (!snake_.isReady) {
		Gui::beginColor(Gui::Gui::eColor::kRed);
		if (ImGui::Button("Ready ?", sf::Vector2f(sizeTexture_, ImGui::GetFrameHeight())) && ptr)
			ptr->changeStateReady(true);
	} else {
		Gui::beginColor(Gui::Gui::eColor::kGreen);
		if (ImGui::Button("Ready !", sf::Vector2f(sizeTexture_, ImGui::GetFrameHeight())) && ptr)
			ptr->changeStateReady(false);
	}
	Gui::endColor();

	renderSelectionColor_(ptr);
}

void WidgetSnake::renderYourSnake_() {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	updateSizeTexture_();
	renderStaticDataSnake_();

	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTexture_) / 2);
	if (!snake_.isReady) {
		Gui::beginColor(Gui::Gui::eColor::kRed);
		if (ImGui::Button("Ready ?", sf::Vector2f(sizeTexture_, ImGui::GetFrameHeight())) && ptr)
			ptr->changeStateReady(true);
	} else {
		Gui::beginColor(Gui::Gui::eColor::kGreen);
		if (ImGui::Button("Ready !", sf::Vector2f(sizeTexture_, ImGui::GetFrameHeight())) && ptr)
			ptr->changeStateReady(false);
	}
	Gui::endColor();

	renderSelectionColor_(ptr);
}
