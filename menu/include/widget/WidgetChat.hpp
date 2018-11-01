#pragma once
#include "AWidget.hpp"

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
	char					_bufferMessage[128];

	WidgetChat &operator=(const WidgetChat&);
	WidgetChat(const WidgetChat&);
};
