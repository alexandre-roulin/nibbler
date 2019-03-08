#include "WidgetMassiveButton.hpp"
#include <cores/Univers.hpp>
#include <gui/Gui.hpp>
#include <network/SnakeServer.hpp>

WidgetMassiveButton::WidgetMassiveButton(Gui &core) :
		AWidget(core, "MassiveButton", NIBBLER_IMGUI_WINDOW_FLAGS_BASIC) {
}

void WidgetMassiveButton::beginContent_() {

	if (button_("create_server()"))
		core_.univers.createServer();
	if (button_("closeAcceptorServer()"))
		core_.univers.getSnakeServer().closeAcceptorServer();
	if (button_("create_client()"))
		core_.univers.createClient();
	if (button_("delete_client()"))
		core_.univers.deleteClient();
}
