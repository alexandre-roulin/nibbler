#include "AWidget.hpp"
#include <gui/Core.hpp>

AWidget::AWidget(Core &core) :
		_core(core),
		_active(true) {
}

AWidget::~AWidget(void) {
}

AWidget::Constructor::~Constructor(void) throw() {}

AWidget::Constructor::Constructor(void) throw() :
		invalid_argument(_error),
		_error("Error during construction") {}

AWidget::Constructor::Constructor(std::string s) throw() :
		invalid_argument(s),
		_error(s) {}

AWidget::Constructor::Constructor(AWidget::Constructor const &src) throw() :
		invalid_argument(_error),
		_error(src._error) { _error = src._error; }

const char *
AWidget::Constructor::what() const throw() { return (_error.c_str()); }
