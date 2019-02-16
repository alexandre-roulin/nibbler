#pragma once

#include "AWidget.hpp"
#include "nibbler.hpp"

class WidgetChat : public AWidget {
public:

	struct colorLog {
		eColorLog color;
		std::string log;

		colorLog(eColorLog acolor, std::string const &&str) :
			color(acolor), log(str) {};
	};

	WidgetChat(Gui &);
	~WidgetChat() override = default;
	WidgetChat() = delete;
	WidgetChat &operator=(const WidgetChat &) = default;
	WidgetChat(const WidgetChat &) = default;

	void addLog(eColorLog color, char const *format, ...);

	void render(void) override;

	void clear(void);

private:
	ImGuiTextBuffer bufferChat_;
	std::vector<colorLog> log_;
	bool scrollChat_;
	char bufferMessage_[CHAT_BUFFER];

	bool chatCommand_(void);

	bool checkClientIsConnect_();
	bool checkClient_();

	void chatText_(void);

	void sendMessage_();
};
