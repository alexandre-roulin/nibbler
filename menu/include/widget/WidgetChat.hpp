#pragma once
#include "AWidget.hpp"
#include "nibbler.hpp"

class WidgetChat : public AWidget {
public:
	WidgetChat(void);
	~WidgetChat(void);


	void	addLog(const char* fmt, ...);

	void	render(void);
	void	clear(void);

private:
	ImGuiTextBuffer			_bufferChat;
	bool					_scrollChat;
	char					_bufferMessage[CHAT_BUFFER];

	WidgetChat &operator=(const WidgetChat&);
	WidgetChat(const WidgetChat&);
};
