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
	AWidget(Gui &);

	virtual ~AWidget(void) = default;

	virtual void render(void) = 0;

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

protected:
	Gui &core_;
	bool active_;

private:
	AWidget &operator=(const AWidget &) = delete;

	AWidget(const AWidget &) = delete;

	AWidget(void) = delete;
};
