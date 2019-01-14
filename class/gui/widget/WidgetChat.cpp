#include "WidgetChat.hpp"
#include <gui/Core.hpp>
#include "imgui.h"
#include <Univers.hpp>

WidgetChat::WidgetChat(Core &core) :
		AWidget(core) {
	bzero(_bufferMessage, IM_ARRAYSIZE(_bufferMessage));
}

WidgetChat::~WidgetChat(void) {}

void WidgetChat::addLog(const char *str, ...) {
	va_list args;

	va_start(args, str);
	_bufferChat.appendfv(str, args);
	_bufferChat.appendfv("\n", args);
	va_end(args);
	_scrollChat = true;
}

void WidgetChat::clear(void) {
	_bufferChat.clear();
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
	ImGui::TextUnformatted(_bufferChat.begin());
	if (_scrollChat)
		ImGui::SetScrollHereY(1.0f);
	_scrollChat = false;
	ImGui::EndChild();
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.8);
	if (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
		ImGui::SetKeyboardFocusHere(0);
	if (ImGui::InputText("Tap", _bufferMessage,
						 IM_ARRAYSIZE(_bufferMessage),
						 ImGuiInputTextFlags_EnterReturnsTrue)) {

		if (!_chatCommand()) {
			_core.univers.getGameNetwork()
					->write_socket(ClientTCP::add_prefix(
							eHeader::CHAT,
							Factory::factory_chat_message(
									_core.univers.getGameNetwork()->getSnake().name,
									_bufferMessage))
					);
		}
		bzero(_bufferMessage, IM_ARRAYSIZE(_bufferMessage));
	}
	ImGui::End();

}

bool WidgetChat::_chatCommand(void) {
	if (_bufferMessage[0] != '/')
		return (false);
	if (strstr(_bufferMessage, "/help"))
		addLog("/help\n/name Aname\n", _bufferMessage);
	else if (strstr(_bufferMessage, "/name "))
		_core.univers.getGameNetwork()->change_name(
				_bufferMessage + sizeof("/name ") - 1);
	else
		addLog("{%s} n'est pas une commande valide\n",
					 _bufferMessage);
	return (true);
}
