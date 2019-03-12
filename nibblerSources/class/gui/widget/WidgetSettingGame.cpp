#include <cores/Univers.hpp>
#include <gui/Gui.hpp>
#include "WidgetSettingGame.hpp"

WidgetSettingGame::WidgetSettingGame(Gui &core) :
		AWidget(core, "Settings Game", NIBBLER_IMGUI_WINDOW_FLAGS_BASIC),
		mapSize_(core_.univers.getMapSize()) {}

void WidgetSettingGame::update_() {
	mapSize_ = core_.univers.getMapSize();
}

void WidgetSettingGame::beginContent_() {

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