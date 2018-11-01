#pragma once
#include "IWidget.hpp"

class WidgetChat : public IWidget {
public:
	WidgetChat(void);
	~WidgetChat(void);


	void	addLog(const char* fmt, ...);

	void	render(void);
	void	clear(void);

private:
	bool					_active;
	ImGuiTextBuffer			_bufferChat;
	bool					_scrollChat;

	WidgetChat &operator=(const WidgetChat&);
	WidgetChat(const WidgetChat&);
};
