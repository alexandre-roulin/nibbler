#include <Univers.hpp>
#include <gui/Core.hpp>
#include "WidgetOption.hpp"

WidgetOption::WidgetOption(Core &core) :
		AWidget(core),
		_mapSize(this->_core.univers.getMapSize()) {

	memcpy(this->_nameBuffer,
		   this->_core.univers.getGameNetwork()->getSnake().name, NAME_BUFFER);
}

WidgetOption::~WidgetOption(void) {}

void WidgetOption::render(void) {
	ImGui::Begin("Options", NULL,
				 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
				 ImGuiWindowFlags_NoCollapse);

	if (ImGui::InputText("Name", this->_nameBuffer,
						 IM_ARRAYSIZE(this->_nameBuffer),
						 ImGuiInputTextFlags_EnterReturnsTrue)) {

		this->_core.univers.getGameNetwork()->change_name(this->_nameBuffer);
		memcpy(this->_nameBuffer,
			   this->_core.univers.getGameNetwork()->getSnake().name,
			   NAME_BUFFER);
	}

	if (ImGui::InputInt("Size map", reinterpret_cast<int *>(&this->_mapSize), 1,
						4, ImGuiInputTextFlags_CharsDecimal |
						   ImGuiInputTextFlags_CharsNoBlank |
						   ImGuiInputTextFlags_EnterReturnsTrue)) {
		if (_mapSize < MAP_MIN)
			this->_mapSize = MAP_MIN;
		else if (this->_mapSize > MAP_MAX)
			this->_mapSize = MAP_MAX;
		this->_core.univers.getGameNetwork()->change_map_size(this->_mapSize);
	}


	if (this->_core.univers.isServer()
		&&
		Snake::allSnakesReady(this->_core.univers.getGameNetwork()->getSnakes(),
							  MAX_SNAKE)) {
		Core::beginColor(Core::HUE_GREEN);
		if (ImGui::Button("Run the game")) {
			this->_core.univers.getGameNetwork()->send_host_open_game();
		}
		Core::endColor();
	}

	ImGui::End();
}