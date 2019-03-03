#include <gui/Gui.hpp>
#include "WidgetEtat.hpp"
#include "cores/Univers.hpp"
#include <network/SnakeServer.hpp>

WidgetEtat::WidgetEtat(Gui &core) :
		AWidget(core, "Etat", NIBBLER_IMGUI_WINDOW_FLAGS_NO_INPUT) {}

void WidgetEtat::beginContent_() {

	SnakeClient::boost_shared_ptr ptrClient(core_.univers.getSnakeClient().lock());
	boost::shared_ptr<ISnakeNetwork> ptrNetwork(core_.univers.getSnakeNetwork().lock());

	if (ptrNetwork) {
		Gui::beginColor(Gui::eColor::kGreen, Gui::eColor::kMedium);
		ImGui::Button("Server Connected", sf::Vector2f(ImGui::GetWindowSize().x / 2, 20));
	} else if (!ptrClient || !ptrClient->isOpen()) {
		Gui::beginColor(Gui::eColor::kRed, Gui::eColor::kMedium);
		ImGui::Button("Server Not connected", sf::Vector2f(ImGui::GetWindowSize().x / 2, 20));
	} else {
		Gui::beginColor(Gui::eColor::kGreen, Gui::eColor::kMedium);
		ImGui::Button("Server Connected", sf::Vector2f(ImGui::GetWindowSize().x / 2, 20));
	}
	Gui::endColor();

	if (ptrNetwork || (ptrClient && ptrClient->isOpen())) {
		if (core_.univers.isServer()) {
			ImGui::SameLine();
			ImGui::Text("You are the host");
		}
	}

	if (!ptrClient) {
		Gui::beginColor(Gui::eColor::kRed, Gui::eColor::kMedium);
		ImGui::Button("Client disable", sf::Vector2f(ImGui::GetWindowSize().x / 2 - 1, 20));
	}
	else if (!ptrClient->isOpen()) {
		Gui::beginColor(Gui::eColor::kGrey, Gui::eColor::kMedium);
		ImGui::Button("Client not connected", sf::Vector2f(ImGui::GetWindowSize().x / 2 - 1, 20));
	}
	else if (ptrClient->isOpen()) {
		Gui::beginColor(Gui::eColor::kGreen, Gui::eColor::kMedium);
		ImGui::Button("Client connected", sf::Vector2f(ImGui::GetWindowSize().x / 2 - 1, 20));
	}
	else
		Gui::beginColor(Gui::eColor::kGreen);
	Gui::endColor();
}
