#include <cores/Univers.hpp>
#include <gui/Gui.hpp>
#include "WidgetServerPannel.hpp"
#include "cores/DisplayDynamicLibrary.hpp"

WidgetServerPannel::WidgetServerPannel(Gui &core) :
		AWidget(core, "Server Pannel", NIBBLER_IMGUI_WINDOW_FLAGS_BASIC) {}

void WidgetServerPannel::beginContent_() {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	if (button_("Generate IA"))
		core_.univers.createBobby();
	if (button_("Delete server"))
		core_.univers.deleteServer();

	if (ptr && ptr->allSnakeIsReady()) {
		if (button_("Run the game", eColor::kGreen)) {
		}
	} else
		button_("Run the game", eColor::kGrey);
}