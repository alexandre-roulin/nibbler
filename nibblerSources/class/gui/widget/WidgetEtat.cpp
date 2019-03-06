#include <gui/Gui.hpp>
#include "WidgetEtat.hpp"
#include "cores/Univers.hpp"
#include <network/SnakeServer.hpp>

WidgetEtat::WidgetEtat(Gui &core) :
		AWidget(core, "Etat", NIBBLER_IMGUI_WINDOW_FLAGS_NO_INPUT) {}

void WidgetEtat::beginContent_() {

	SnakeClient::boost_shared_ptr ptrClient(core_.univers.getMySnakeClient().lock());
	boost::shared_ptr<ISnakeNetwork> ptrNetwork(core_.univers.getSnakeNetwork().lock());

	if (ptrNetwork)
		button_("Server Connected", ImGui::GetWindowSize().x / 2, eColor::kGreen, eColor::kMedium);
	else if (!ptrClient || !ptrClient->isOpen())
		button_("Server Not connected", ImGui::GetWindowSize().x / 2, eColor::kRed, eColor::kMedium);
	else
		button_("Server Connected", ImGui::GetWindowSize().x / 2, eColor::kGreen, eColor::kMedium);

	if (!ptrClient)
		button_("Client disable", ImGui::GetWindowSize().x / 2, eColor::kRed, eColor::kMedium);
	else if (!ptrClient->isOpen())
		button_("Client not connected", ImGui::GetWindowSize().x / 2, eColor::kGrey, eColor::kMedium);
	else if (ptrClient->isOpen())
		button_("Client connected", ImGui::GetWindowSize().x / 2, eColor::kGreen, eColor::kMedium);


	if (ptrNetwork || (ptrClient && ptrClient->isOpen()))
		if (core_.univers.isServer())
			ImGui::Text("You are the host");
}
