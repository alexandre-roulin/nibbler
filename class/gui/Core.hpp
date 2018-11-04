#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>
#include "widget/WidgetChat.hpp"
#include "nibbler.hpp"

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

	Core(Univers &univers);
	~Core(void);

	//				GUI				//
	void						demo(void);
	bool						titleScreen(void);
	void						aState(void);
	void						addMessageChat(std::string const &);
	void 						exit(void);
	sf::Vector2<unsigned int>	positionByPercent(sf::Vector2<unsigned int> const &percent) const;

	Univers						&univers;
private:
	//				GUI				//
	sf::Vector2<unsigned int>	_winSize;
	sf::RenderWindow			_win;
	ImGuiIO						&_io;
	sf::Texture					_imageTitleScreen;
	sf::Clock					_deltaClock;
	WidgetChat					_chat;

	void						_render(void);
	ImGuiIO						&_createContext(void);
	void						_processEvent(sf::Event const &event);

	Core          &operator=(Core const &rhs);
	Core(Core const &src);
};
