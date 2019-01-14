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
	}
	if (ImGui::Button("create_server()")) {
	}
	if (ImGui::Button("delete_server()")) {
	}
	if (ImGui::Button("create_client()")) {
	}
	if (ImGui::Button("delete_server()")) {
	}
	if (ImGui::Button("start_game()")) {
	}
	if (ImGui::Button("borderless()")) {
	}

	ImGui::End();
}
