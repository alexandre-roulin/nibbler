#pragma once

#include "AWidget.hpp"
#include "nibbler.hpp"

class Gui;

class WidgetOption : public AWidget {
public:
	WidgetOption(Gui &);
	~WidgetOption() override = default;
	WidgetOption() = delete;
	WidgetOption &operator=(const WidgetOption &) = delete;
	WidgetOption(const WidgetOption &) = delete;

private:
	char nameBuffer_[NAME_BUFFER];
	bool sound_;
	bool &rNoise_;
	bool &rMusique_;
	std::string pathSound_;
	int indexLibrary_;

	void update_() override;
	void beginContent_() override;
};
