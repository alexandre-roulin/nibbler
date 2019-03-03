#include "WidgetMassiveButton.hpp"
#include <cores/Univers.hpp>
#include <gui/Gui.hpp>
#include <network/SnakeServer.hpp>

WidgetMassiveButton::WidgetMassiveButton(Gui &core) :
		AWidget(core, "MassiveButton", NIBBLER_IMGUI_WINDOW_FLAGS_BASIC) {
}

void WidgetMassiveButton::beginContent_() {
	if (ImGui::Button("create_server()"))
		core_.univers.createServer();
	if (ImGui::Button("create_client()"))
		core_.univers.createClient();
	if (ImGui::Button("delete_client()"))
		core_.univers.deleteClient();
}
