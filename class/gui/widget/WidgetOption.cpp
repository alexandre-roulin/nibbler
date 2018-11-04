#include "WidgetOption.hpp"
#include "Univers.hpp"
#include "../../../imgui-sfml/imgui.h"

WidgetOption::WidgetOption(Core &core) :
AWidget(core)
{
}

WidgetOption::~WidgetOption(void)
{}

void			WidgetOption::render(void)
{
	ImGui::Begin("Chat", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::End();

}
