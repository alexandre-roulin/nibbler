#include "WidgetChat.hpp"
#include "Univers.hpp"

WidgetChat::WidgetChat(Univers &univers) :
AWidget(),
_univers(univers)
{
	bzero(this->_bufferMessage, IM_ARRAYSIZE(this->_bufferMessage));
}

WidgetChat::~WidgetChat(void)
{}

void			WidgetChat::addLog(const char* str, ...)
{
	int			old_size;
	va_list		args;

	old_size = this->_bufferChat.size();
	va_start(args, str);
	this->_bufferChat.appendfv(str, args);
	this->_bufferChat.appendfv("\n", args);
	va_end(args);
	this->_scrollChat = true;
}
void			WidgetChat::clear(void)
{
	this->_bufferChat.clear();
}

void			WidgetChat::render(void)
{
	ImGui::Begin("Chat", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	if (ImGui::Button("Clear"))
		this->clear();
	ImGui::BeginChild("scrolling", ImVec2(0,ImGui::GetWindowHeight() - 4 * ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::TextUnformatted(this->_bufferChat.begin());
	if (this->_scrollChat)
		ImGui::SetScrollHereY(1.0f);
	this->_scrollChat = false;
	ImGui::EndChild();
	if (ImGui::InputText("Tap", this->_bufferMessage, IM_ARRAYSIZE(this->_bufferMessage), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		std::string bufferMessage;
		ClientTCP::add_prefix(CHAT, bufferMessage, this->_bufferMessage,
							  sizeof(_bufferMessage));
		this->_univers.getClientTCP_().write_socket(bufferMessage);
		bzero(this->_bufferMessage, IM_ARRAYSIZE(this->_bufferMessage));
	}
	ImGui::End();

}
