#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>
#include <boost/filesystem.hpp>
#include "widget/WidgetChat.hpp"
#include "nibbler.hpp"
#include <Univers.hpp>

class Gui {
public:

	class CoreConstructorException : public std::exception {
	public:
		CoreConstructorException(void) noexcept;
		CoreConstructorException(std::string) noexcept;
		virtual const char* what() const noexcept;
		~CoreConstructorException(void) noexcept;
		CoreConstructorException(CoreConstructorException const &src) noexcept;
	private:
		CoreConstructorException &operator=(CoreConstructorException const &rhs) noexcept;
		std::string			_error;
	};

	Gui(Univers &univers);
	~Gui(void);

	//				GUI				//
	void						titleScreen(void);
	void						aState(void);
	template < typename ... Args >
	void addMessageChat(eColorLog color, std::string const &log, Args ... args) {
		_chat.addLog(color, log, args...);
	}
	template < typename ... Args >
	void addMessageChat(std::string const &log, Args ... args) {
		_chat.addLog(eColorLog::kNone, log, args...);
	}

	void 						exit(void);
	sf::Vector2<unsigned int>	positionByPercent(sf::Vector2<unsigned int> const &percent) const;

	Univers						&univers;

	boost::filesystem::path const	&getPathRessources() const;

	static void						beginColor(float const color);
	static void						endColor();

	static float const 				HUE_RED;
	static float const 				HUE_GREEN;

private:
	//				GUI				//
	boost::filesystem::path		pathRessources_;

	sf::Vector2<unsigned int>	_winSize;
	sf::RenderWindow			_win;
	ImGuiIO						&_io;
	sf::Texture					_imageTitleScreen;
	sf::Clock					_deltaClock;
	WidgetChat					_chat;
	sf::Vector2<int>			_mapSize;

	void						_render(void);
	ImGuiIO						&_createContext(void);
	void						_processEvent(sf::Event const &event);
	void						_updateGenCoreEvent();

	Gui          &operator=(Gui const &rhs);
	Gui(Gui const &src);

	static bool 				_useColor;
};
