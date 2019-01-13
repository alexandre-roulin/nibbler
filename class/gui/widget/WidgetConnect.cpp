#include <gui/Core.hpp>
#include "WidgetConnect.hpp"
#include "Univers.hpp"
#include "imgui.h"

WidgetConnect::WidgetConnect(Core &core) :
		AWidget(core),
		_client(false)
{
	bzero(this->_dnsBuffer, IM_ARRAYSIZE(this->_dnsBuffer));
	bzero(this->_portBuffer, IM_ARRAYSIZE(this->_portBuffer));
	memcpy(this->_dnsBuffer, "localhost", sizeof("localhost"));
	memcpy(this->_portBuffer, "4242", sizeof("4242"));

}

WidgetConnect::~WidgetConnect(void)
{}

void			WidgetConnect::render(void)
{

	if (this->_core.univers.getGameNetwork()->isConnect())
		ImGui::Begin("Connect", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	else
		ImGui::Begin("Connect", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

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
		if (ImGui::Button("Join", sf::Vector2f(ImGui::GetWindowSize().x, 20))) {
			this->_core.univers.getGameNetwork()->connect(this->_dnsBuffer, this->_portBuffer);
		}

	}
	else {
		if (ImGui::Button("Create", sf::Vector2f(ImGui::GetWindowSize().x, 20))) {
			this->_core.univers.create_server();
			this->_core.univers.getGameNetwork()->connect(this->_dnsBuffer, this->_portBuffer);
		}
	}
	Core::endColor();



	ImGui::End();
}

