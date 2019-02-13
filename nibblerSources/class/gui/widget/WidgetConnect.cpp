#include <gui/Gui.hpp>
#include "WidgetConnect.hpp"
#include "Univers.hpp"

WidgetConnect::WidgetConnect(Gui &core) :
		AWidget(core),
		client_(false) {
	bzero(dnsBuffer_, IM_ARRAYSIZE(dnsBuffer_));
	bzero(portBuffer_, IM_ARRAYSIZE(portBuffer_));
	memcpy(dnsBuffer_, "localhost", sizeof("localhost"));
	memcpy(portBuffer_, "4242", sizeof("4242"));

}

void WidgetConnect::render(void) {

	if (core_.univers.getSnakeClient() && core_.univers.getSnakeClient()->isConnect())
		ImGui::Begin("Connect", NULL,
					 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	else
		ImGui::Begin("Connect", NULL,
					 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	if (client_)
		Gui::beginColor(Gui::HUE_RED);
	else
		Gui::beginColor(Gui::HUE_GREEN);
	if (ImGui::Button("Host", sf::Vector2f(ImGui::GetWindowSize().x / 2, 20)))
		client_ = false;
	ImGui::SameLine();
	Gui::endColor();

	if (client_)
		Gui::beginColor(Gui::HUE_GREEN);
	else
		Gui::beginColor(Gui::HUE_RED);
	if (ImGui::Button("Client", sf::Vector2f(ImGui::GetWindowSize().x / 2 - 1, 20)))
		client_ = true;

	Gui::endColor();


	if (ImGui::InputText("DNS", dnsBuffer_,
						 IM_ARRAYSIZE(dnsBuffer_),
						 ImGuiInputTextFlags_EnterReturnsTrue)) {

	}
	if (ImGui::InputText("PORT", portBuffer_,
						 IM_ARRAYSIZE(portBuffer_),
						 ImGuiInputTextFlags_EnterReturnsTrue)) {

	}

	ImGui::Spacing();
	ImGui::Spacing();

	Gui::beginColor(Gui::HUE_GREEN);
	if (client_) {
		if (ImGui::Button("Join", sf::Vector2f(ImGui::GetWindowSize().x, 20)) && core_.univers.getSnakeClient()) {
			core_.univers.getSnakeClient()->connect(dnsBuffer_, portBuffer_);
		}

	} else {
		if (ImGui::Button("Create", sf::Vector2f(ImGui::GetWindowSize().x, 20)) && core_.univers.getSnakeClient()) {
			core_.univers.create_server();
			core_.univers.getSnakeClient()->connect(dnsBuffer_, portBuffer_);
		}
	}
	Gui::endColor();


	ImGui::End();
}

