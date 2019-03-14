#include <cores/Univers.hpp>
#include <gui/Gui.hpp>
#include "WidgetSettingGame.hpp"

WidgetSettingGame::WidgetSettingGame(Gui &core) :
		AWidget(core, "Settings Game", NIBBLER_IMGUI_WINDOW_FLAGS_BASIC),
		mapSize_(core_.univers.getMapSize()),
		speed(core_.univers.getBaseSpeed()) {}

void WidgetSettingGame::update_() {
	mapSize_ = core_.univers.getMapSize();
}

void WidgetSettingGame::beginContent_() {

	if (ImGui::BeginCombo("Difficulty", getDifficultyDisplay_(speed).c_str(), 0)) {
		for (size_t n = 0; n < 4; ++n) {
			bool is_selected = difficultyDisplay[n] == getDifficultyDisplay_(speed);
			if (ImGui::Selectable(difficultyDisplay[n].c_str(), is_selected)) {
				core_.univers.setBaseSpeed(getSpeedEnum(difficultyDisplay[n]));
				boost::shared_ptr<ISnakeNetwork> ptr(core_.univers.getSnakeNetwork().lock());
				if (ptr)
					ptr->notifyGameSpeed();
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	if (ImGui::InputInt("Size map", reinterpret_cast<int *>(&mapSize_), 1,
						4, ImGuiInputTextFlags_CharsDecimal |
						   ImGuiInputTextFlags_CharsNoBlank |
						   ImGuiInputTextFlags_EnterReturnsTrue)) {
		if (mapSize_ < MAP_MIN)
			mapSize_ = MAP_MIN;
		else if (mapSize_ > MAP_MAX)
			mapSize_ = MAP_MAX;

		core_.univers.setMapSize(mapSize_);
		core_.univers.updateSizeMap();
	}


	bool borderless = core_.univers.isBorderless();
	if (ImGui::Checkbox("Borderless", &borderless))
		core_.univers.switchBorderless();

}

const std::string &
WidgetSettingGame::getDifficultyDisplay_(const GameManager::eSpeed s) {
	switch (s) {
		case GameManager::Easy:
			return difficultyDisplay[0];
		case GameManager::Medium:
			return difficultyDisplay[1];
		case GameManager::Hard:
			return difficultyDisplay[2];
		case GameManager::Impossible:
			return difficultyDisplay[3];
		default:
			exit(1);
	}
}

GameManager::eSpeed WidgetSettingGame::getSpeedEnum(const std::string &string) {
	if (difficultyDisplay[0] == string)
		return GameManager::eSpeed::Easy;
	if (difficultyDisplay[2] == string)
		return GameManager::eSpeed::Hard;
	if (difficultyDisplay[3] == string)
		return GameManager::eSpeed::Impossible;
	return GameManager::eSpeed::Medium;
}

const std::array<std::string, 4> WidgetSettingGame::difficultyDisplay = {
		{ "Easy",
				"Medium",
				"Hard",
				"Impossible" }
};