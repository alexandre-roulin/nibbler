#pragma once
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

class Core {
	public:

	class CoreConstructorException : public std::exception {
	public:
		CoreConstructorException(void) throw();
		CoreConstructorException(std::string) throw();
		virtual const char* what() const throw();
		~CoreConstructorException(void) throw();
		CoreConstructorException(CoreConstructorException const &src) throw();
	private:
		CoreConstructorException &operator=(CoreConstructorException const &rhs) throw();
		std::string			_error;
	};

	Core(void);
	~Core(void);
	bool		TitleScreen(void);
	void		aState(void);

private:
	sf::RenderWindow		_win;
	bool					_my_tool_active;
	sf::Texture				_background;
	sf::Clock				_deltaClock;

	Core          &operator=(Core const &rhs);
	Core(Core const &src);
};
