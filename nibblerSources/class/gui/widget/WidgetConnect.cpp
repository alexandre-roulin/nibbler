#include <gui/Core.hpp>
#include "WidgetConnect.hpp"
#include "Univers.hpp"
#include "imgui.h"
#include <network/SnakeClient.hpp>
WidgetConnect::WidgetConnect(Core &core) :
		AWidget(core),
		_client(false)
{
	bzero(_dnsBuffer, IM_ARRAYSIZE(_dnsBuffer));
	bzero(_portBuffer, IM_ARRAYSIZE(_portBuffer));
	memcpy(_dnsBuffer, "localhost", sizeof("localhost"));
	memcpy(_portBuffer, "4242", sizeof("4242"));

}

WidgetConnect::~WidgetConnect(void)
{}

void			WidgetConnect::render(void)
{

	if (_core.univers.getSnakeClient() && _core.univers.getSnakeClient()->isConnect())
		ImGui::Begin("Connect", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	else
		ImGui::Begin("Connect", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	if (_client)
		Core::beginColor(Core::HUE_RED);
	else
		Core::beginColor(Core::HUE_GREEN);
	if (ImGui::Button("Host", sf::Vector2f(ImGui::GetWindowSize().x / 2, 20)))
		_client = false;
	ImGui::SameLine();
	Core::endColor();

	if (_client)
		Core::beginColor(Core::HUE_GREEN);
	else
		Core::beginColor(Core::HUE_RED);
	if (ImGui::Button("Client", sf::Vector2f(ImGui::GetWindowSize().x / 2 - 1, 20)))
		_client = true;

	Core::endColor();


	if (ImGui::InputText("DNS", _dnsBuffer,
						 IM_ARRAYSIZE(_dnsBuffer),
						 ImGuiInputTextFlags_EnterReturnsTrue)) {

	}
	if (ImGui::InputText("PORT", _portBuffer,
						 IM_ARRAYSIZE(_portBuffer),
						 ImGuiInputTextFlags_EnterReturnsTrue)) {

	}

	ImGui::Spacing();
	ImGui::Spacing();

	Core::beginColor(Core::HUE_GREEN);
	if (_client) {
		if (ImGui::Button("Join", sf::Vector2f(ImGui::GetWindowSize().x, 20)) && _core.univers.getSnakeClient()) {
			_core.univers.getSnakeClient()->connect(_dnsBuffer, _portBuffer);
		}

	}
	else {
		if (ImGui::Button("Create", sf::Vector2f(ImGui::GetWindowSize().x, 20)) && _core.univers.getSnakeClient()) {
			_core.univers.create_server();
			_core.univers.getSnakeClient()->connect(_dnsBuffer, _portBuffer);
		}
	}
	Core::endColor();



	ImGui::End();
}

