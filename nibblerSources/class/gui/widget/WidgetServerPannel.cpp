#include <cores/Univers.hpp>
#include <gui/Gui.hpp>
#include "WidgetServerPannel.hpp"
#include "cores/ExternalLibraryDisplayManager.hpp"

WidgetServerPannel::WidgetServerPannel(Gui &core) :
		AWidget(core, "Server Pannel", NIBBLER_IMGUI_WINDOW_FLAGS_BASIC) {}

void WidgetServerPannel::beginContent_() {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	if (ImGui::Button("Generate IA"))
		core_.univers.createBobby();
	if (ImGui::Button("Delete server"))
		core_.univers.deleteServer();

	if (ptr && ptr->allSnakeIsReady()) {
		Gui::beginColor(Gui::eColor::kGreen);
		if (ImGui::Button("Run the game")) {
		}
	} else {
		Gui::beginColor(Gui::eColor::kGrey);
		ImGui::Button("Run the game");
	}
	Gui::endColor();
}