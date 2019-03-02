#include <cores/Univers.hpp>
#include <gui/Gui.hpp>
#include "WidgetServerPannel.hpp"
#include "cores/ExternalLibraryDisplayManager.hpp"

WidgetServerPannel::WidgetServerPannel(Gui &core) :
		AWidget(core) {}

void WidgetServerPannel::content_(bool renderContentInWindow) {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	ImGui::Begin("Server Pannel", NULL,
				 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
				 ImGuiWindowFlags_NoCollapse);

	if (ImGui::Button("create_ia()")) {
		core_.univers.createBobby();
	}
	if (ImGui::Button("delete_server()")) {
		core_.univers.deleteServer();
	}

	if (ptr && ptr->allSnakeIsReady()) {
		Gui::beginColor(Gui::eColor::kGreen);
		if (ImGui::Button("Run the game")) {
		}
	} else {
		Gui::beginColor(Gui::eColor::kGrey);
		ImGui::Button("Run the game");
	}
	Gui::endColor();

	ImGui::End();
}