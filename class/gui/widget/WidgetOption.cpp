#include <gui/Core.hpp>
#include "WidgetOption.hpp"
#include "Univers.hpp"
#include "imgui.h"

WidgetOption::WidgetOption(Core &core) :
AWidget(core)
{
	this->_mapSize[0] = 30;
	this->_mapSize[1] = 30;
	memcpy(this->_nameBuffer, this->_core.univers.getClientTCP_().getSnake().name, NAME_BUFFER);
}

WidgetOption::~WidgetOption(void)
{}

void			WidgetOption::render(void)
{
	ImGui::Begin("Options", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	if (ImGui::InputText("Name", this->_nameBuffer,
						 IM_ARRAYSIZE(this->_nameBuffer),
						 ImGuiInputTextFlags_EnterReturnsTrue)) {

		this->_core.univers.getClientTCP_().change_name(this->_nameBuffer);
		memcpy(this->_nameBuffer, this->_core.univers.getClientTCP_().getSnake().name, NAME_BUFFER);
	}

	if (ImGui::InputInt2("Size map", this->_mapSize, ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue)) {
		if (this->_mapSize[0] < MAP_MIN)
			this->_mapSize[0] = MAP_MIN;
		else if (this->_mapSize[0] > MAP_MAX)
			this->_mapSize[0] = MAP_MAX;
		if (this->_mapSize[1] < MAP_MIN)
			this->_mapSize[1] = MAP_MIN;
		else if (this->_mapSize[1] > MAP_MAX)
			this->_mapSize[1] = MAP_MAX;
	}



	ImGui::End();
}