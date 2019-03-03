#include "WidgetExit.hpp"

WidgetExit::WidgetExit(Gui &core, std::function<void(Gui&)> callbackExit) :
		AWidget(core, "Exit", ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground),
		callbackExit_(callbackExit)
{}

void WidgetExit::beginContent_() {
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
}
