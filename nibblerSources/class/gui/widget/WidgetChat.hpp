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

	~WidgetChat(void) override = default;

	void addLog(eColorLog color, char const *format, ...)
	{
		va_list args;
		va_start(args, format);
		size_t size = std::vsnprintf(nullptr, 0, format, args) + 1;
		va_end(args);

		std::unique_ptr<char[]> buf = std::make_unique<char[]>(size);

		va_start(args, format);
		std::vsnprintf(buf.get(), size, format, args);
		va_end(args);

		log_.emplace_back(color, std::string(buf.get(), buf.get() + size - 1));
		scrollChat_ = true;
	}

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

	WidgetChat &operator=(const WidgetChat &) = delete;

	WidgetChat(const WidgetChat &) = delete;

	WidgetChat(void) = delete;
};
