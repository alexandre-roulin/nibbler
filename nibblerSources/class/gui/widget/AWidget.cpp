#include "AWidget.hpp"
#include <gui/Gui.hpp>

AWidget::AWidget(Gui &core) :
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


void AWidget::render(bool renderContentInWindow) {
	if (!renderContentInWindow) {
		ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(100, 100, 100, 100));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(100, 100, 100, 100));
		ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, IM_COL32(100, 100, 100, 100));
		ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(100, 100, 100, 100));
	}
	content_(renderContentInWindow);
	if (!renderContentInWindow)
		ImGui::PopStyleColor(4);
}