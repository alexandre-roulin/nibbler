#pragma once

#include "AWidget.hpp"

class WidgetExit : public AWidget {
public:
	WidgetExit(Core &);

	WidgetExit(Core &, void (*callback)(void *), void *ptr = nullptr);

	~WidgetExit(void) override = default;


	void render(void) override;

private:
	void (*callback_)(void *);

	void *ptr_;

	WidgetExit &operator=(const WidgetExit &) = delete;

	WidgetExit(const WidgetExit &) = delete;

	WidgetExit(void) = delete;
};
