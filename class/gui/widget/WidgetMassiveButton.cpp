#include "WidgetMassiveButton.hpp"
#include <Univers.hpp>
#include <gui/Core.hpp>

WidgetMassiveButton::WidgetMassiveButton(Core &core) :
		AWidget(core)
{
}

WidgetMassiveButton::~WidgetMassiveButton(void)
{
}

void			WidgetMassiveButton::render(void)
{
	ImGui::Begin("MassiveButton", nullptr,
				 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
				 ImGuiWindowFlags_NoCollapse);

	if (ImGui::Button("create_ia()")) {
		_core.univers.create_ia();
	}
	if (ImGui::Button("delete_ia()")) {
		_core.univers.delete_ia();
	}
	if (ImGui::Button("create_server()")) {
		_core.univers.create_server();
	}
	if (ImGui::Button("delete_server()")) {
		_core.univers.delete_server();
	}
	if (ImGui::Button("create_client()")) {
		_core.univers.create_client();
		_core.univers.getGameNetwork()->connect("localhost", "4242");
	}
	if (ImGui::Button("delete_client()")) {
		_core.univers.delete_client();
	}
	if (ImGui::Button("start_game()")) {
		_core.univers.new_game();
	}
	if (ImGui::Button("borderless()")) {
		_core.univers.setBorderless(!_core.univers.isBorderless());
	}


	ImGui::End();
}
