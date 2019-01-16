#include <Univers.hpp>
#include <gui/Core.hpp>
#include "WidgetOption.hpp"

WidgetOption::WidgetOption(Core &core) :
		AWidget(core),
		_mapSize(_core.univers.getMapSize()) {

	memcpy(_nameBuffer,
		   _core.univers.getGameNetwork()->getSnake().name, NAME_BUFFER);
}

WidgetOption::~WidgetOption(void) {}

void WidgetOption::render(void) {
	ImGui::Begin("Options", NULL,
				 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
				 ImGuiWindowFlags_NoCollapse);

	if (ImGui::InputText("Name", _nameBuffer,
						 IM_ARRAYSIZE(_nameBuffer),
						 ImGuiInputTextFlags_EnterReturnsTrue)) {

		_core.univers.getGameNetwork()->change_name(_nameBuffer);
		memcpy(_nameBuffer,
			   _core.univers.getGameNetwork()->getSnake().name,
			   NAME_BUFFER);
	}

	if (ImGui::InputInt("Size map", reinterpret_cast<int *>(&_mapSize), 1,
						4, ImGuiInputTextFlags_CharsDecimal |
						   ImGuiInputTextFlags_CharsNoBlank |
						   ImGuiInputTextFlags_EnterReturnsTrue)) {
		if (_mapSize < MAP_MIN)
			_mapSize = MAP_MIN;
		else if (_mapSize > MAP_MAX)
			_mapSize = MAP_MAX;
		_core.univers.getGameNetwork()->change_map_size(_mapSize);
	}


	if (_core.univers.isServer()
		&&
		Snake::allSnakesReady(_core.univers.getGameNetwork()->getSnakes(),
							  MAX_SNAKE)) {
		Core::beginColor(Core::HUE_GREEN);
		if (ImGui::Button("Run the game")) {
			_core.univers.getGameNetwork()->send_host_open_game();
		}
		Core::endColor();
	}

	ImGui::End();
}