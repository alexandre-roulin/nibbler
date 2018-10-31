#include "WidgetChat.hpp"

WidgetChat::WidgetChat(void) :
_active(true)
{}

WidgetChat::~WidgetChat(void)
{}

void			WidgetChat::addLog(const char* str, ...)
{
	int			old_size;
	va_list		args;

	old_size = this->_bufferChat.size();
	va_start(args, str);
	this->_bufferChat.appendfv(str, args);
	va_end(args);
	this->_scrollChat = true;
}
void			WidgetChat::clear(void)
{
	this->_bufferChat.clear();
}


void			WidgetChat::render(void)
{
	ImGui::SetNextWindowPosCenter();
	ImGui::Begin("Chat", &this->_active, ImGuiWindowFlags_MenuBar);

	ImGui::TextColored(ImVec4(1,1,0,1), "Chat");
	if (ImGui::Button("Clear"))
		this->clear();
	ImGui::BeginChild("scrolling", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::TextUnformatted(this->_bufferChat.begin());

	if (this->_scrollChat)
		ImGui::SetScrollHereY(1.0f);
	this->_scrollChat = false;
	ImGui::EndChild();
	ImGui::End();

}
