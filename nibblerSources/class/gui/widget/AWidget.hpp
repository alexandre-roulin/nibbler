#pragma once

#include "IWidget.hpp"

class Gui;

enum class eColorLog {
	kNone,
	kRed,
	kGreen,
	kBlue,
	kPink,
	kOrange,
	kYellow
};

class AWidget : public IWidget {
public:
	class Constructor : public std::invalid_argument {
	public:
		Constructor(void) noexcept;

		Constructor(std::string const &s) noexcept;

		const char *what() const noexcept override;

		~Constructor(void) noexcept = default;

		Constructor(Constructor const &src) noexcept;

	private:
		Constructor &operator=(Constructor const &rhs) noexcept = delete;

		std::string error_;
	};

	AWidget(Gui &);
	virtual ~AWidget(void) = default;
	AWidget() = delete;
	AWidget &operator=(const AWidget &) = default;
	AWidget(const AWidget &) = default;


	virtual void render(void) = 0;


protected:
	Gui &core_;
	bool active_;
};
