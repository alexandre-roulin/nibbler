#include <Univers.hpp>
#include <gui/Core.hpp>
#include "WidgetOption.hpp"

WidgetOption::WidgetOption(Core &core) :
		AWidget(core),
		mapSize_(core_.univers.getMapSize()) {

	memcpy(nameBuffer_,
		   core_.univers.getSnakeClient()->getSnake().name, NAME_BUFFER);
}

void WidgetOption::render(void) {
	ImGui::Begin("Options", NULL,
				 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
				 ImGuiWindowFlags_NoCollapse);

	if (ImGui::InputText("Name", nameBuffer_,
						 IM_ARRAYSIZE(nameBuffer_),
						 ImGuiInputTextFlags_EnterReturnsTrue)) {

		core_.univers.getSnakeClient()->changeName(nameBuffer_);
		memcpy(nameBuffer_,
			   core_.univers.getSnakeClient()->getSnake().name,
			   NAME_BUFFER);
	}

	if (ImGui::InputInt("Size map", reinterpret_cast<int *>(&mapSize_), 1,
						4, ImGuiInputTextFlags_CharsDecimal |
						   ImGuiInputTextFlags_CharsNoBlank |
						   ImGuiInputTextFlags_EnterReturnsTrue)) {
		if (mapSize_ < MAP_MIN)
			mapSize_ = MAP_MIN;
		else if (mapSize_ > MAP_MAX)
			mapSize_ = MAP_MAX;
		core_.univers.getSnakeClient()->changeMapSize(mapSize_);
	}

	if (core_.univers.isServer()
		&& core_.univers.getSnakeClient()->allSnakeIsReady()) {
		Core::beginColor(Core::HUE_GREEN);
		if (ImGui::Button("Run the game")) {
			core_.univers.getSnakeClient()->sendHostOpenGame();
		}
		Core::endColor();
	}

	ImGui::End();
}