#include <cores/Univers.hpp>
#include <gui/Gui.hpp>
#include "WidgetSettingGame.hpp"

WidgetSettingGame::WidgetSettingGame(Gui &core) :
		AWidget(core),
		mapSize_(core_.univers.getMapSize()) {}

void WidgetSettingGame::content_(bool renderContentInWindow) {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	ImGui::Begin("Settings Game", NULL,
				 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
				 ImGuiWindowFlags_NoCollapse);

	if (ImGui::InputInt("Size map", reinterpret_cast<int *>(&mapSize_), 1,
						4, ImGuiInputTextFlags_CharsDecimal |
						   ImGuiInputTextFlags_CharsNoBlank |
						   ImGuiInputTextFlags_EnterReturnsTrue) && ptr) {
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

	ImGui::End();
}