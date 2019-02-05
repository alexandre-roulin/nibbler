#pragma once

#include "AWidget.hpp"
#include "nibbler.hpp"
#include <network/ClientTCP.hpp>

class WidgetChat : public AWidget {
public:
	WidgetChat(Core &);

	~WidgetChat(void) override = default;


	void addLog(const char *fmt, ...);

	void render(void) override;

	void clear(void);

private:
	ImGuiTextBuffer bufferChat_;
	bool scrollChat_;
	char bufferMessage_[CHAT_BUFFER];

	bool chatCommand_(void);

	WidgetChat &operator=(const WidgetChat &) = delete;

	WidgetChat(const WidgetChat &) = delete;

	WidgetChat(void) = delete;
};
