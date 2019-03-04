#include "AWidget.hpp"
#include <gui/Gui.hpp>

AWidget::AWidget(Gui &core, std::string const &winName, ImGuiWindowFlags winFlags) :
		core_(core),
		active_(true),
		winName_(winName),
		winFlags_(winFlags),
		sizeButton_(ImVec2(100, 20)) {
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


void AWidget::update_() {}

void AWidget::render(bool renderContentInWindow) {
	update_();
	if (active_) {
		if (!renderContentInWindow) {
			ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(100, 100, 100, 100));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(100, 100, 100, 100));
			ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, IM_COL32(100, 100, 100, 100));
			ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(100, 100, 100, 100));
		}
		ImGui::Begin(winName_.c_str(), NULL, winFlags_);
		if (renderContentInWindow)
			beginContent_();
		if (!renderContentInWindow)
			ImGui::PopStyleColor(4);
		ImGui::End();
	}
}


bool AWidget::button_(std::string const &text, int sizeTextX,  eColor color, eColor power) const {
	if (color != eColor::kNone)
		Gui::beginColor(color, power);
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - sizeTextX) / 2);
	bool ret =  ImGui::Button(text.c_str(), ImVec2(sizeTextX, sizeButton_.y));
	if (color != eColor::kNone)
		Gui::endColor();
	return ret;
}

bool AWidget::button_(std::string const &text,  eColor color, eColor power) const {
	return button_(text, sizeButton_.x, color, power);
}