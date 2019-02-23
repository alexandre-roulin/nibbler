#include "WidgetChat.hpp"
#include <gui/Gui.hpp>
#include <cores/Test.hpp>

WidgetChat::WidgetChat(Gui &core) :
		AWidget(core) {
	bzero(bufferMessage_, IM_ARRAYSIZE(bufferMessage_));
}

void WidgetChat::clear() {
	bufferChat_.clear();
}

void WidgetChat::render() {
	ImGui::Begin("Chat", NULL,
				 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
				 ImGuiWindowFlags_NoCollapse);
	if (ImGui::Button("Clear"))
		clear();
	ImGui::BeginChild("scrolling", ImVec2(0, ImGui::GetWindowHeight() - 4 * ImGui::GetFrameHeightWithSpacing()), false,
					  ImGuiWindowFlags_HorizontalScrollbar);
	for (auto const &log : log_) {
		switch (log.color) {
			case eColorLog::kNone:
				ImGui::Text("%s", log.log.c_str());
				break;
			case eColorLog::kRed:
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, .9f), "%s", log.log.c_str());
				break;
			case eColorLog::kGreen:
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, .9f), "%s", log.log.c_str());
				break;
			case eColorLog::kBlue:
				ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, .9f), "%s", log.log.c_str());
				break;
			case eColorLog::kPink:
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, .9f), "%s", log.log.c_str());
				break;
			case eColorLog::kOrange:
				ImGui::TextColored(ImVec4(1.0f, .6f, 0.0f, .9f), "%s", log.log.c_str());
				break;
			case eColorLog::kYellow:
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, .9f), "%s", log.log.c_str());
				break;
		}
	}
	if (scrollChat_)
		ImGui::SetScrollHereY(1.0f);
	scrollChat_ = false;
	ImGui::EndChild();
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.8);
	if (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
		ImGui::SetKeyboardFocusHere(0);
	if (ImGui::InputText("Tap", bufferMessage_, IM_ARRAYSIZE(bufferMessage_), ImGuiInputTextFlags_EnterReturnsTrue)) {
		sendMessage_();
	}
	ImGui::End();

}

bool WidgetChat::checkClient_() {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	if (!ptr) {
		addLog(eColorLog::kRed, "Your Client should be created");
		return (false);
	}
	return (true);
}

bool WidgetChat::checkClientIsConnect_() {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());
	if (!ptr || !checkClient_())
		return (false);
	if (!ptr->isOpen()) {
		addLog(eColorLog::kRed, "Your Client should be connected");
		return (false);
	}
	return (true);
}

void WidgetChat::sendMessage_() {
	if (!chatCommand_())
		chatText_();
	memset(bufferMessage_, 0, IM_ARRAYSIZE(bufferMessage_));
}

void WidgetChat::chatText_() {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	if (ptr && checkClientIsConnect_()) {
		ptr->sendDataToServer(ChatInfo(ptr->getSnake().name, bufferMessage_), eHeader::kChat);
	}
}

bool WidgetChat::chatCommand_() {
	SnakeClient::boost_shared_ptr ptr(core_.univers.getSnakeClient().lock());

	if (bufferMessage_[0] != '/')
		return (false);
	if (strstr(bufferMessage_, "/help"))
		addLog(eColorLog::kYellow, "/help\n/name Aname\n");
	else if (strstr(bufferMessage_, "/name ")) {
		if (ptr && checkClientIsConnect_()) {
			ptr->changeName(bufferMessage_ + sizeof("/name ") - 1);
		}
	} else if (strstr(bufferMessage_, "/color")) {
		addLog(eColorLog::kNone, "eColorLog::kNone");
		addLog(eColorLog::kRed, "eColorLog::kRed");
		addLog(eColorLog::kGreen, "eColorLog::kGreen");
		addLog(eColorLog::kBlue, "eColorLog::kBlue");
		addLog(eColorLog::kPink, "eColorLog::kPink");
		addLog(eColorLog::kOrange, "eColorLog::kOrange");
		addLog(eColorLog::kYellow, "eColorLog::kYellow");
	} else
		addLog(eColorLog::kOrange, "{%s} n'est pas une commande valide\n",
			   bufferMessage_);
	return (true);
}

void WidgetChat::addLog(eColorLog color, char const *format, ...)
{
	va_list args;
	va_start(args, format);
	size_t size = std::vsnprintf(nullptr, 0, format, args) + 1;
	va_end(args);

	std::unique_ptr<char[]> buf = std::make_unique<char[]>(size);

	va_start(args, format);
	std::vsnprintf(buf.get(), size, format, args);
	va_end(args);

	log_.emplace_back(color, std::string(buf.get(), buf.get() + size - 1));
	Test::getInstance().addLog(buf.get());
	scrollChat_ = true;
}