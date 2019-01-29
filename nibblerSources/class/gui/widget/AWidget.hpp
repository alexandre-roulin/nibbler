#pragma once
#include "IWidget.hpp"

class Core;

class AWidget : public IWidget {
public:
	AWidget(Core &);
	virtual ~AWidget(void);

	virtual void	render(void) = 0;

	class Constructor : public std::invalid_argument {
	public:
		Constructor(void) throw();
		Constructor(std::string) throw();
		virtual const char* what() const throw();
		~Constructor(void) throw();
		Constructor(Constructor const &src) throw();
	private:
		Constructor &operator=(Constructor const &rhs) throw();
		std::string			_error;
	};

protected:
	Core										&_core;
	bool										_active;

private:
	AWidget &operator=(const AWidget&);
	AWidget(const AWidget&);
	AWidget(void);
};
