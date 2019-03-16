#pragma once

#include "AWidget.hpp"
#include "nibbler.hpp"

class WidgetChat : public AWidget {
public:

	struct colorLog {
		eColorLog color;
		std::string log;

		colorLog(eColorLog acolor, std::string const &str);
	};

	WidgetChat(Gui &);
	~WidgetChat() override = default;
	WidgetChat() = delete;
	WidgetChat &operator=(const WidgetChat &) = default;
	WidgetChat(const WidgetChat &) = default;

	void addLog(eColorLog color, std::string const &log);

private:
	ImGuiTextBuffer bufferChat_;
	std::vector<colorLog> log_;
	bool scrollChat_;
	char bufferMessage_[CHAT_BUFFER];
	std::mutex mutex_;

	void addLog_(eColorLog color, std::string const &log);

	void beginContent_() override;
	bool chatCommand_();
	bool checkClientIsConnect_();
	bool checkClient_();
	void chatText_();
	void sendMessage_();
};
