#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>
#include <boost/filesystem.hpp>
#include "widget/WidgetChat.hpp"
#include "nibbler.hpp"
#include <cores/Univers.hpp>
#include <iostream>
#include <fstream>
#include "GuiData.hpp"

#define GUI_INPUT_DIRECTORY "input"

class Gui {
public:

	class CoreConstructorException : public std::exception {
	public:
		CoreConstructorException() noexcept;
		CoreConstructorException(std::string) noexcept;
		virtual const char* what() const noexcept;
		~CoreConstructorException() noexcept;
		CoreConstructorException(CoreConstructorException const &src) noexcept;
	private:
		CoreConstructorException &operator=(CoreConstructorException const &rhs) noexcept;
		std::string			error_;
	};

	Gui(Univers &univers);
	~Gui(void);
	Gui() = delete;
	Gui &operator=(const Gui &) = delete;
	Gui(const Gui &) = delete;

	void						mainLoop();
	template < typename ... Args >
	void addMessageChat(eColorLog color, std::string const &log, Args ... args) {
		chat_.addLog(color, log.c_str(), args...);
	}
	template < typename ... Args >
	void addMessageChat(std::string const &log, Args ... args) {
		chat_.addLog(eColorLog::kNone, log.c_str(), args...);
	}

	void 						exit();
	sf::Vector2<unsigned int>	positionByPercent(sf::Vector2<unsigned int> const &percent) const;

	Univers						&univers;

	boost::filesystem::path const	&getPathRessources() const;

	static void						beginColor(eColor color, eColor power = eColor::kHard);
	static void						endColor();

private:

	//				GUI				//
	boost::filesystem::path		pathRessources_;

	sf::Vector2<unsigned int>	winSize_;
	sf::RenderWindow			win_;
	ImGuiIO						&io_;
	sf::Clock					deltaClock_;
	WidgetChat					chat_;

	void						updateEvent_();
	void						render_();
	ImGuiIO						&createContext_();
	void						processEvent_(sf::Event const &event);

	static void					beginHueColor_(float color, float power = 1.f);
	static bool 				useColor_;
	static std::map< eColor, float > const mapEColor_;
};
