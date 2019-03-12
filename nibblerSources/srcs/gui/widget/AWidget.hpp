#pragma once

#include "IWidget.hpp"
#include <gui/GuiData.hpp>

#define NIBBLER_IMGUI_WINDOW_FLAGS_BASIC (ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)
#define NIBBLER_IMGUI_WINDOW_FLAGS_NO_INPUT (ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMouseInputs)

class Gui;

class AWidget : public IWidget {
public:
	class Constructor : public std::invalid_argument {
	public:
		Constructor() noexcept;
		Constructor(std::string const &s) noexcept;
		const char *what() const noexcept override;
		~Constructor() noexcept = default;
		Constructor(Constructor const &src) noexcept;
	private:
		Constructor &operator=(Constructor const &rhs) noexcept = delete;
		std::string error_;
	};

	AWidget(Gui &core, std::string const &winName, ImGuiWindowFlags winFlags);
	virtual ~AWidget(void) = default;
	AWidget() = delete;
	AWidget &operator=(const AWidget &) = default;
	AWidget(const AWidget &) = default;


	void render(bool renderContentInWindow);

protected:
	Gui &core_;
	bool active_;
	std::string winName_;
	ImGuiWindowFlags winFlags_;
	ImVec2 sizeButton_;

	bool button_(std::string const &text, eColor color = eColor::kNone, eColor power = eColor::kHard) const;
	bool button_(std::string const &text, int sizeTextX, eColor color = eColor::kNone, eColor power = eColor::kHard) const;

private:
	virtual void update_();
	virtual void beginContent_() = 0;
};
