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

	if (ImGui::Button("create_ia()")) {
		core_.univers.create_ia();
	}
	if (ImGui::Button("delete_ia()")) {
		core_.univers.delete_ia();
	}
	if (ImGui::Button("create_server()")) {
		core_.univers.create_server();
	}
	if (ImGui::Button("delete_server()")) {
		core_.univers.deleteServer();
	}
	if (ImGui::Button("create_client()")) {
		core_.univers.create_client();
	}
	if (ImGui::Button("connect()")) {
//		if (core_.univers.getSnakeClient() && !core_.univers.isOnlyIA())
//			core_.univers.getSnakeClient()->connect("e1r5p15.42.fr", "4242");
	}
	if (ImGui::Button("delete_client()")) {
		core_.univers.deleteClient();
	}
	if (ImGui::Button("start_game()")) {
//		if (core_.univers.isServer() && core_.univers.getSnakeClient()) {
//		} else {
//			core_.addMessageChat("FAIS PAS LA MERDE GROS ! T'es pas un server OU TA PAS DE JOUEUR");
//		}

//		gui_.setStartGame(true);
		//gui_.univers.load_extern_lib_display(Univers::EXTERN_LIB_SFML);
		//gui_.univers.new_game();
		//gui_.univers.new_game();
	}
	if (ImGui::Button("borderless()")) {
		core_.univers.setBorderless(!core_.univers.isBorderless());
	}


	ImGui::End();
}
