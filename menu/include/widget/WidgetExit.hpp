#pragma once
#include "IWidget.hpp"

class WidgetExit : public IWidget {
public:
	WidgetExit(void);
	WidgetExit(void (*callback)(void *), void *ptr);
	~WidgetExit(void);


	void	render(void);

private:
	bool					_active;
	void					(*_callback)(void *);
	void					*_ptr;

	WidgetExit &operator=(const WidgetExit&);
	WidgetExit(const WidgetExit&);
};
