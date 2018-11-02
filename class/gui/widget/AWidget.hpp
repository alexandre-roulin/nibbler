#pragma once
#include "IWidget.hpp"

class AWidget : public IWidget {
public:
	AWidget(void);
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
	bool										_active;

private:
	AWidget &operator=(const AWidget&);
	AWidget(const AWidget&);
};
