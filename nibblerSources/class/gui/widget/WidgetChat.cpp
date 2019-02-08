#include "WidgetChat.hpp"
#include <gui/Core.hpp>
#include <Univers.hpp>

WidgetChat::WidgetChat(Core &core) :
		AWidget(core) {
	bzero(bufferMessage_, IM_ARRAYSIZE(bufferMessage_));
}

void WidgetChat::addLog(const char *str, ...) {
	va_list args;

	va_start(args, str);
	bufferChat_.appendfv(str, args);
	bufferChat_.appendfv("\n", args);
	va_end(args);
	scrollChat_ = true;
}

void WidgetChat::clear(void) {
	bufferChat_.clear();
}

void WidgetChat::render(void) {
	ImGui::Begin("Chat", NULL,
				 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
				 ImGuiWindowFlags_NoCollapse);
	if (ImGui::Button("Clear"))
		clear();
	ImGui::BeginChild("scrolling", ImVec2(0, ImGui::GetWindowHeight() - 4 *
																		ImGui::GetFrameHeightWithSpacing()),
					  false, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::TextUnformatted(bufferChat_.begin());
	if (scrollChat_)
		ImGui::SetScrollHereY(1.0f);
	scrollChat_ = false;
	ImGui::EndChild();
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.8);
	if (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
		ImGui::SetKeyboardFocusHere(0);
	if (ImGui::InputText("Tap", bufferMessage_,
						 IM_ARRAYSIZE(bufferMessage_),
						 ImGuiInputTextFlags_EnterReturnsTrue)) {

		if (!chatCommand_()) {
			core_.univers.getSnakeClient()->sendDataToServer(ChatInfo(
					core_.univers.getSnakeClient()->getSnake().name,
					bufferMessage_
			), eHeader::kChat);
		}
		bzero(bufferMessage_, IM_ARRAYSIZE(bufferMessage_));
	}
	ImGui::End();

}

bool WidgetChat::chatCommand_(void) {
	if (bufferMessage_[0] != '/')
		return (false);
	if (strstr(bufferMessage_, "/help"))
		addLog("/help\n/name Aname\n", bufferMessage_);
	else if (strstr(bufferMessage_, "/name "))
		core_.univers.getSnakeClient()->changeName(
				bufferMessage_ + sizeof("/name ") - 1);
	else
		addLog("{%s} n'est pas une commande valide\n",
			   bufferMessage_);
	return (true);
}