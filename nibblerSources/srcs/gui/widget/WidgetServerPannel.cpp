#include <cores/Univers.hpp>
#include <gui/Gui.hpp>
#include "WidgetServerPannel.hpp"
#include "dynamicLinkLibrary/DisplayDynamicLibrary.hpp"
#include "network/SnakeServer.hpp"

WidgetServerPannel::WidgetServerPannel(Gui &core) :
		AWidget(core, "Server Pannel", NIBBLER_IMGUI_WINDOW_FLAGS_BASIC) {}

void WidgetServerPannel::beginContent_() {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	if (button_("Generate IA"))
		core_.univers.createBobby();
	if (button_("Delete server"))
		core_.univers.deleteServer();

	if (core_.univers.isServer() && button_("Close Accept Server"))
		core_.univers.getSnakeServer().closeAcceptorServer();

	if (ptr && ptr->allSnakeIsReady()) {
		if (core_.univers.isServer() && button_("Run the game", eColor::kGreen)) {
			core_.univers.sendOpenGameToServer();
		}
	} else
		button_("Run the game", eColor::kGrey);
}