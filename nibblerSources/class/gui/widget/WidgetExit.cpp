#include "WidgetExit.hpp"

WidgetExit::WidgetExit(Gui &core, std::function<void(Gui&)> callbackExit) :
		AWidget(core),
		callbackExit_(callbackExit)
{}

void WidgetExit::render(void) {
	ImGui::Begin("Exit", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
	if (ImGui::Button("Exit.."))
		ImGui::OpenPopup("Exit ?");
	if (ImGui::BeginPopupModal("Exit ?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Do you want Exit the game ?");
		ImGui::Separator();


		if (ImGui::Button("OK", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
			callbackExit_(core_);
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	ImGui::End();
}
