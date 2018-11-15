#include <gui/Core.hpp>
#include "WidgetOption.hpp"
#include "Univers.hpp"
#include "imgui.h"

WidgetOption::WidgetOption(Core &core) :
AWidget(core)
{
	memcpy(this->_nameBuffer, this->_core.univers.getClientTCP_().getSnake().name, NAME_BUFFER);
}

WidgetOption::~WidgetOption(void)
{}

void			WidgetOption::render(void)
{
	ImGui::Begin("TEST", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
//	ImGui::InputInt("Size of Map", &i0);
//	ImGui::InputInt2("input int2", vec4i);
	void change_name(char const *name);


	if (ImGui::InputText("Name", this->_nameBuffer,
						 IM_ARRAYSIZE(this->_nameBuffer),
						 ImGuiInputTextFlags_EnterReturnsTrue)) {

		this->_core.univers.getClientTCP_().change_name(this->_nameBuffer);
		memcpy(this->_nameBuffer, this->_core.univers.getClientTCP_().getSnake().name, NAME_BUFFER);
	}
	ImGui::End();

}
