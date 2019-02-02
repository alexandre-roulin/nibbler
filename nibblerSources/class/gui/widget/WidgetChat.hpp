#pragma once
#include "AWidget.hpp"
#include "nibbler.hpp"
#include <network/ClientTCP.hpp>

class WidgetChat : public AWidget {
public:
	WidgetChat(Core &);
	~WidgetChat(void);


	void	addLog(const char* fmt, ...);

	void	render(void);
	void	clear(void);

private:
	ImGuiTextBuffer			_bufferChat;
	bool					_scrollChat;
	char					_bufferMessage[CHAT_BUFFER];

	bool					_chatCommand(void);

	WidgetChat &operator=(const WidgetChat&);
	WidgetChat(const WidgetChat&);
	WidgetChat(void);
};