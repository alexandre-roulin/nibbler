#include <gui/Core.hpp>
#include "WidgetOption.hpp"
#include "Univers.hpp"
#include "imgui.h"

WidgetOption::WidgetOption(Core &core) :
AWidget(core),
_client(false)
{
	memcpy(this->_nameBuffer, this->_core.univers.getClientTCP_().getSnake().name, NAME_BUFFER);
	bzero(this->_dnsBuffer, IM_ARRAYSIZE(this->_dnsBuffer));
	bzero(this->_portBuffer, IM_ARRAYSIZE(this->_portBuffer));
}

WidgetOption::~WidgetOption(void)
{}

void			WidgetOption::render(void)
{
	static ImVec4 color = ImColor(114, 144, 154, 200);

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

	ImGui::Spacing();
	ImGui::Spacing();


	if (this->_client)
		Core::beginColor(Core::HUE_RED);
	else
		Core::beginColor(Core::HUE_GREEN);
	if (ImGui::Button("Host", sf::Vector2f(ImGui::GetWindowSize().x / 2, 20)))
		this->_client = false;
	ImGui::SameLine();
	Core::endColor();

	if (this->_client)
		Core::beginColor(Core::HUE_GREEN);
	else
		Core::beginColor(Core::HUE_RED);
	if (ImGui::Button("Client", sf::Vector2f(ImGui::GetWindowSize().x / 2 - 1, 20)))
		this->_client = true;
	Core::endColor();





	if (ImGui::InputText("DNS", this->_dnsBuffer,
						 IM_ARRAYSIZE(this->_dnsBuffer),
						 ImGuiInputTextFlags_EnterReturnsTrue)) {

	}
	if (ImGui::InputText("PORT", this->_portBuffer,
						 IM_ARRAYSIZE(this->_portBuffer),
						 ImGuiInputTextFlags_EnterReturnsTrue)) {

	}

	ImGui::Spacing();
	ImGui::Spacing();

	Core::beginColor(Core::HUE_GREEN);
	if (this->_client) {
		ImGui::Button("Join", sf::Vector2f(ImGui::GetWindowSize().x, 20));
	}
	else {
		ImGui::Button("Create", sf::Vector2f(ImGui::GetWindowSize().x, 20));
	}
	Core::endColor();



	ImGui::End();

}
