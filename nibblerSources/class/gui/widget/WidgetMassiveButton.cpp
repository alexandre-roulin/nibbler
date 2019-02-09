#include "WidgetMassiveButton.hpp"
#include <Univers.hpp>
#include <gui/Core.hpp>
#include <network/SnakeServer.hpp>

WidgetMassiveButton::WidgetMassiveButton(Core &core) :
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
		if (core_.univers.getSnakeClient() && !core_.univers.isOnlyIA())
			core_.univers.getSnakeClient()->connect("e1r5p15.42.fr", "4242");
	}
	if (ImGui::Button("delete_client()")) {
		core_.univers.deleteClient();
	}
	if (ImGui::Button("start_game()")) {
		if (core_.univers.isServer() && core_.univers.getSnakeClient()) {
		} else {
			core_.addMessageChat("FAIS PAS LA MERDE GROS ! T'es pas un server OU TA PAS DE JOUEUR");
		}

//		core_.setStartGame(true);
		//core_.univers.load_extern_lib_display(Univers::EXTERN_LIB_SFML);
		//core_.univers.new_game();
		//core_.univers.new_game();
	}
	if (ImGui::Button("borderless()")) {
		core_.univers.setBorderless(!core_.univers.isBorderless());
	}


	ImGui::End();
}
