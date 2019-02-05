#pragma once

#include "AWidget.hpp"
#include "nibbler.hpp"

class Core;

class WidgetOption : public AWidget {
public:
	WidgetOption(Core &);

	~WidgetOption(void) override = default;


	void render(void) override;

private:
	unsigned int mapSize_;
	char nameBuffer_[NAME_BUFFER];

	WidgetOption &operator=(const WidgetOption &) = delete;

	WidgetOption(const WidgetOption &) = delete;
};
