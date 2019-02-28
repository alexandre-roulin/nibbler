#include "WidgetMassiveButton.hpp"
#include <cores/Univers.hpp>
#include <gui/Gui.hpp>
#include <network/SnakeServer.hpp>

WidgetMassiveButton::WidgetMassiveButton(Gui &core) :
		AWidget(core) {
}

void WidgetMassiveButton::render(void) {
	ImGui::Begin("MassiveButton", nullptr,
				 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
				 ImGuiWindowFlags_NoCollapse);

	if (ImGui::Button("create_server()")) {
		core_.univers.createServer();
	}
	if (ImGui::Button("create_client()")) {
		core_.univers.createClient();
	}
	if (ImGui::Button("delete_client()")) {
		core_.univers.deleteClient();
	}

	ImGui::End();
}
