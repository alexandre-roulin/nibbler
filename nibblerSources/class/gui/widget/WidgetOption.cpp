#include <Univers.hpp>
#include <gui/Core.hpp>
#include "WidgetOption.hpp"
#include <network/SnakeClient.hpp>

WidgetOption::WidgetOption(Core &core) :
		AWidget(core),
		_mapSize(_core.univers.getMapSize()) {

	memcpy(_nameBuffer,
		   _core.univers.getSnakeClient()->getSnake().name, NAME_BUFFER);
}

WidgetOption::~WidgetOption(void) {}

void WidgetOption::render(void) {
	ImGui::Begin("Options", NULL,
				 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
				 ImGuiWindowFlags_NoCollapse);

	if (ImGui::InputText("Name", _nameBuffer,
						 IM_ARRAYSIZE(_nameBuffer),
						 ImGuiInputTextFlags_EnterReturnsTrue)) {

		_core.univers.getSnakeClient()->changeName(_nameBuffer);
		memcpy(_nameBuffer,
			   _core.univers.getSnakeClient()->getSnake().name,
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
		_core.univers.getSnakeClient()->changeMapSize(_mapSize);
	}

	if (_core.univers.isServer()
		&& _core.univers.getSnakeClient()->allSnakeIsReady()) {
		Core::beginColor(Core::HUE_GREEN);
		if (ImGui::Button("Run the game")) {
			_core.univers.getSnakeClient()->sendHostOpenGame();
		}
		Core::endColor();
	}

	ImGui::End();
}