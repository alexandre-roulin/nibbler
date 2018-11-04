#include "WidgetExit.hpp"

WidgetExit::WidgetExit(Core &core) :
AWidget(core),
_callback(nullptr),
_ptr(nullptr)
{}
WidgetExit::WidgetExit(Core &core, void (*callback)(void *), void *ptr = nullptr) :
AWidget(core),
_callback(callback),
_ptr(ptr)
{}

WidgetExit::~WidgetExit(void)
{}

void			WidgetExit::render(void)
{
	ImGui::Begin("Exit", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
	if (ImGui::Button("Exit.."))
		ImGui::OpenPopup("Exit ?");
	if (ImGui::BeginPopupModal("Exit ?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Do you want Exit the game ?");
		ImGui::Separator();


		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			if (this->_callback)
				this->_callback(this->_ptr);
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	ImGui::End();
}
