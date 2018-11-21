#include "Core.hpp"
#include "widget/WidgetExit.hpp"
#include "widget/WidgetSnake.hpp"
#include "widget/WidgetLobby.hpp"
#include "gui/widget/WidgetOption.hpp"
#include "gui/widget/WidgetConnect.hpp"
#include <vector>
#include <iostream>
#include <logger.h>


Core::Core(Univers &univers) :
univers(univers),
_winSize(sf::Vector2<unsigned int>(1000, 900)),
_win(sf::VideoMode(this->_winSize.x, this->_winSize.y), "Project Sanke"),
_io(this->_createContext()),
_chat(*this),
_mapSize(sf::Vector2<int>(20, 20))
{
	if (!this->_imageTitleScreen.loadFromFile("ressource/ecran_titre.png"))
		(throw(Core::CoreConstructorException("Cannot load background")));
	this->_io.IniFilename = NULL;
}

ImGuiIO			&Core::_createContext(void)
{
	this->_win.setFramerateLimit(60);
	ImGui::SFML::Init(this->_win);
	ImGui::CreateContext();
	return (ImGui::GetIO());
}

Core::~Core(void)
{
	ImGui::SFML::Shutdown();
}

void			Core::_updateGenCoreEvent() {
	sf::Event event;
	while (this->_win.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);

		if (event.type == sf::Event::Closed)
			this->_win.close();
	}
	ImGui::SFML::Update(this->_win, this->_deltaClock.restart());
}

void			Core::titleScreen()
{
	sf::Event	event;
	bool		titleScreen = true;

	while (this->_win.isOpen() && titleScreen)
	{
		while (this->_win.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
				this->_win.close();
			else if (event.type == sf::Event::KeyPressed)
				titleScreen = false;
		}
		ImGui::SFML::Update(this->_win, this->_deltaClock.restart());
		ImGui::SetNextWindowPosCenter();
		ImGui::Begin("Titlescreen", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove);
		ImGui::Image(this->_imageTitleScreen);
		ImGui::End();
		this->_render();
	}
}

void			Core::mainMenu() {
	int clickSolo = 0;
	int clickMulti = 0;
	sf::Vector2f sizeButton(50, 20);

	while (this->_win.isOpen())
	{
		this->_updateGenCoreEvent();


		ImGui::SetNextWindowPos(this->positionByPercent(sf::Vector2<unsigned int>(0, 0)));
		ImGui::SetNextWindowSize(this->positionByPercent(sf::Vector2<unsigned int>(50, 100)));
		ImGui::Begin("Multi", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

		ImGui::SetCursorPos(sf::Vector2f(ImGui::GetWindowSize().x / 2 - sizeButton.x / 2,
										 ImGui::GetWindowSize().y / 2 - sizeButton.y / 2));
		if (ImGui::Button("Multi", sizeButton))
			clickMulti++;
		if (clickMulti & 1)
		{

		}
		ImGui::End();

		ImGui::SetNextWindowPos(this->positionByPercent(sf::Vector2<unsigned int>(50, 0)));
		ImGui::SetNextWindowSize(this->positionByPercent(sf::Vector2<unsigned int>(50, 100)));
		ImGui::Begin("Solo", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);


		ImGui::SetCursorPos(sf::Vector2f(ImGui::GetWindowSize().x / 2 - sizeButton.x / 2,
				                         ImGui::GetWindowSize().y / 2 - sizeButton.y / 2));
		if (ImGui::Button("Solo", sizeButton))
			clickSolo++;
		if (clickSolo & 1)
		{
			ImGui::SameLine();
			ImGui::Text("Thanks for clicking me!");
		}
		ImGui::End();

		this->_render();
	}
}

void			Core::demo(void)
{
	while (this->_win.isOpen())
	{
		this->_updateGenCoreEvent();

		ImGui::ShowDemoWindow();
		this->_render();
	}
}

void			callbackExit(void *ptr)
{
	static_cast<Core *>(ptr)->exit();
}

void			Core::aState(void)
{
	WidgetExit wexit(*this, &callbackExit, this);
	WidgetLobby *lobby = nullptr;
	WidgetOption *optionSnake = nullptr;
	WidgetConnect optionConnect(*this);


	lobby = new WidgetLobby(*this, this->univers.getClientTCP_().getSnakes());
	while (this->_win.isOpen() && !this->univers.getClientTCP_().isOpenGame())
	{
		sf::Event event;
		while (this->_win.pollEvent(event))
		{
			this->_processEvent(event);
			ImGui::SFML::ProcessEvent(event);
		}

		ImGui::SFML::Update(this->_win, this->_deltaClock.restart());

		ImGui::SetNextWindowPos(this->positionByPercent(sf::Vector2<unsigned int>(0, 50)));
		ImGui::SetNextWindowSize(this->positionByPercent(sf::Vector2<unsigned int>(70, 50)));
		this->_chat.render();

		lobby->render();

		ImGui::SetNextWindowPos(this->positionByPercent(sf::Vector2<unsigned int>(95, 0)), 0, sf::Vector2f(0.5f, 0.5f));
		wexit.render();

		if (this->univers.getClientTCP_().isConnect()) {
			if (!optionSnake) {
				if (lobby)
					delete lobby;
				lobby = new WidgetLobby(*this, this->univers.getClientTCP_().getSnakes());
				optionSnake = new WidgetOption(*this);
			}
			ImGui::SetNextWindowPos(this->positionByPercent(sf::Vector2<unsigned int>(70, 50)));
			ImGui::SetNextWindowSize(this->positionByPercent(sf::Vector2<unsigned int>(30, 25)));
			optionSnake->render();
			ImGui::SetNextWindowPos(this->positionByPercent(sf::Vector2<unsigned int>(70, 75)));
			ImGui::SetNextWindowSize(this->positionByPercent(sf::Vector2<unsigned int>(30, 25)));
			optionConnect.render();
		}
		else {
			ImGui::SetNextWindowPos(this->positionByPercent(sf::Vector2<unsigned int>(70, 50)));
			ImGui::SetNextWindowSize(this->positionByPercent(sf::Vector2<unsigned int>(30, 50)));
			optionConnect.render();
		}

		this->_render();
	}
	if (optionSnake)
		delete optionSnake;
	if (lobby)
		delete lobby;
}

void				Core::addMessageChat(std::string const &msg)
{
	this->_chat.addLog(msg.c_str());
}

void				Core::_render(void)
{
	this->_win.clear();
	ImGui::SFML::Render(this->_win);
	this->_win.display();
}

void 				Core::exit(void)
{
	this->_win.close();
}

sf::Vector2<unsigned int>	Core::positionByPercent(sf::Vector2<unsigned int> const &percent) const
{
	return (sf::Vector2<unsigned int>(this->_winSize.x * percent.x / 100,
										this->_winSize.y * percent.y / 100));
}

void					Core::_processEvent(sf::Event const &event)
{
	if (event.type == sf::Event::Resized)
		this->_winSize = sf::Vector2<unsigned int>(event.size.width, event.size.height);
	else if (event.type == sf::Event::Closed)
		this->_win.close();
}


void					Core::beginColor(float const color) {
	assert(Core::_useColor == false);
	Core::_useColor = true;
	ImGui::PushStyleColor(ImGuiCol_Button, static_cast<ImVec4>(ImColor::HSV(color, 0.7f, 0.7f)));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, static_cast<ImVec4>(ImColor::HSV(color, 0.8f, 0.8f)));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, static_cast<ImVec4>(ImColor::HSV(color, 0.9f, 0.9f)));
}
void					Core::endColor() {
	ImGui::PopStyleColor(3);
	Core::_useColor = false;
}


float const 				Core::HUE_GREEN = 0.33f;
float const 				Core::HUE_RED = 0.f;

bool 						Core::_useColor = false;

Core::CoreConstructorException::~CoreConstructorException(void) throw(){}
Core::CoreConstructorException::CoreConstructorException(void) throw() :
	_error("Error on Core constructor") {}
Core::CoreConstructorException::CoreConstructorException(std::string s) throw() :
	_error(s) { }
Core::CoreConstructorException::CoreConstructorException(Core::CoreConstructorException const &src) throw() :
	_error(src._error)
	{ this->_error = src._error; }
const char	*Core::CoreConstructorException::what() const throw()
	{ return (this->_error.c_str()); }

