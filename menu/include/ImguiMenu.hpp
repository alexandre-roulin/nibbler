#pragma once
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

class ImguiMenu {
	public:

	class ImguiMenuConstructorException : public std::exception {
	public:
		ImguiMenuConstructorException(void) throw();
		ImguiMenuConstructorException(std::string) throw();
		virtual const char* what() const throw();
		~ImguiMenuConstructorException(void) throw();
		ImguiMenuConstructorException(ImguiMenuConstructorException const &src) throw();
	private:
		ImguiMenuConstructorException &operator=(ImguiMenuConstructorException const &rhs) throw();
		std::string			_error;
	};

	ImguiMenu(void);
	~ImguiMenu(void);
	void		aState(void);

private:
	sf::RenderWindow		_win;
	bool					_my_tool_active;
	sf::Texture				_background;
	sf::Clock				_deltaClock;

	ImguiMenu          &operator=(ImguiMenu const &rhs);
	ImguiMenu(ImguiMenu const &src);
};
