#pragma once

#include "IWidget.hpp"

class Core;

class AWidget : public IWidget {
public:
	AWidget(Core &);

	virtual ~AWidget(void) = default;

	virtual void render(void) = 0;

	class Constructor : public std::invalid_argument {
	public:
		Constructor(void) noexcept;

		Constructor(std::string const &s) noexcept;

		virtual const char *what() const noexcept;

		~Constructor(void) noexcept = default;

		Constructor(Constructor const &src) noexcept;

	private:
		Constructor &operator=(Constructor const &rhs) noexcept = delete;

		std::string error_;
	};

protected:
	Core &core_;
	bool active_;

private:
	AWidget &operator=(const AWidget &) = delete;

	AWidget(const AWidget &) = delete;

	AWidget(void) = delete;
};
