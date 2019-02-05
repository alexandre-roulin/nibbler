#include "WidgetExit.hpp"

WidgetExit::WidgetExit(Core &core) :
		AWidget(core),
		callback_(nullptr),
		ptr_(nullptr) {}

WidgetExit::WidgetExit(Core &core, void (*callback)(void *), void *ptr) :
		AWidget(core),
		callback_(callback),
		ptr_(ptr) {}

void WidgetExit::render(void) {
	ImGui::Begin("Exit", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
	if (ImGui::Button("Exit.."))
		ImGui::OpenPopup("Exit ?");
	if (ImGui::BeginPopupModal("Exit ?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Do you want Exit the game ?");
		ImGui::Separator();


		if (ImGui::Button("OK", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
			if (callback_)
				callback_(ptr_);
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
