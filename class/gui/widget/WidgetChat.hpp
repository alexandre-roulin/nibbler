#pragma once
#include "AWidget.hpp"
#include "nibbler.hpp"
#include <network/ClientTCP.hpp>

class WidgetChat : public AWidget {
public:
	WidgetChat(Univers &univers);
	~WidgetChat(void);


	void	addLog(const char* fmt, ...);

	void	render(void);
	void	clear(void);

private:
	Univers					&_univers;
	ImGuiTextBuffer			_bufferChat;
	bool					_scrollChat;
	char					_bufferMessage[CHAT_BUFFER];

	WidgetChat &operator=(const WidgetChat&);
	WidgetChat(const WidgetChat&);
	WidgetChat(void);
};
