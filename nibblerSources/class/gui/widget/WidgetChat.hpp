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

	WidgetChat(Core &);

	~WidgetChat(void) override = default;

	template < typename ... Args >
	void addLog(eColorLog color, std::string const &log, Args ... args) {
		size_t size = std::snprintf(nullptr, 0, log.c_str(), args ...) + 1;
		std::unique_ptr<char[]> buf = std::make_unique<char[]>(size);
		std::snprintf(buf.get(), size, log.c_str(), args ...);
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
