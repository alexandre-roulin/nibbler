#pragma once
#include "AWidget.hpp"

class WidgetExit : public AWidget {
public:
	WidgetExit(Core &);
	WidgetExit(Core &, void (*callback)(void *), void *ptr);
	~WidgetExit(void);


	void	render(void);

private:
	void					(*_callback)(void *);
	void					*_ptr;

	WidgetExit &operator=(const WidgetExit&);
	WidgetExit(const WidgetExit&);
	WidgetExit(void);
};
