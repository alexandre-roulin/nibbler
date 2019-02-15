#include <cores/Univers.hpp>
#include <gui/Gui.hpp>
#include "WidgetOption.hpp"

WidgetOption::WidgetOption(Gui &core) :
		AWidget(core),
		mapSize_(core_.univers.getMapSize()) {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	if (ptr) memcpy(nameBuffer_, ptr->getSnake().name, NAME_BUFFER);
}

void WidgetOption::render(void) {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	ImGui::Begin("Options", NULL,
				 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
				 ImGuiWindowFlags_NoCollapse);

	if (ImGui::InputText("Name", nameBuffer_,
						 IM_ARRAYSIZE(nameBuffer_),
						 ImGuiInputTextFlags_EnterReturnsTrue) && ptr) {

		ptr->changeName(nameBuffer_);
		memcpy(nameBuffer_,
			   ptr->getSnake().name,
			   NAME_BUFFER);
	}

	if (ImGui::InputInt("Size map", reinterpret_cast<int *>(&mapSize_), 1,
						4, ImGuiInputTextFlags_CharsDecimal |
						   ImGuiInputTextFlags_CharsNoBlank |
						   ImGuiInputTextFlags_EnterReturnsTrue) && ptr) {
		if (mapSize_ < MAP_MIN)
			mapSize_ = MAP_MIN;
		else if (mapSize_ > MAP_MAX)
			mapSize_ = MAP_MAX;
		ptr->changeMapSize(mapSize_);
	}

	if (core_.univers.isServer() && ptr && ptr->allSnakeIsReady()) {
		Gui::beginColor(Gui::HUE_GREEN);
		if (ImGui::Button("Run the game")) {
			ptr->sendHostOpenGame();
		}
		Gui::endColor();
	}

	ImGui::End();
}