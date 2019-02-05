#include "AWidget.hpp"
#include <gui/Core.hpp>

AWidget::AWidget(Core &core) :
		core_(core),
		active_(true) {
}

AWidget::Constructor::Constructor(void) noexcept :
		invalid_argument(error_),
		error_("Error during construction") {}

AWidget::Constructor::Constructor(std::string const &s) noexcept :
		invalid_argument(s),
		error_(s) {}

AWidget::Constructor::Constructor(AWidget::Constructor const &src) noexcept :
		invalid_argument(error_),
		error_(src.error_) { error_ = src.error_; }

const char *
AWidget::Constructor::what() const noexcept { return (error_.c_str()); }
