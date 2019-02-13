#include <Univers.hpp>
#include "Gui.hpp"
#include "widget/WidgetExit.hpp"
#include "widget/WidgetSnake.hpp"
#include "widget/WidgetLobby.hpp"
#include "gui/widget/WidgetOption.hpp"
#include "gui/widget/WidgetConnect.hpp"
#include "gui/widget/WidgetMassiveButton.hpp"

Gui::Gui(Univers &univers) :
univers(univers),
pathRessources_(boost::filesystem::path(NIBBLER_ROOT_PROJECT_PATH) / "ressources/"),
_winSize(sf::Vector2<unsigned int>(1000, 900)),
_win(sf::VideoMode(_winSize.x, _winSize.y), "Project Sanke"),
_io(_createContext()),
_chat(*this),
_mapSize(sf::Vector2<int>(20, 20))
{
	_io.IniFilename = NULL;
}

ImGuiIO			&Gui::_createContext(void)
{
	_win.setFramerateLimit(60);
	ImGui::SFML::Init(_win);

	// Give Focus on Widow
	sf::Event event;
	event.type = sf::Event::GainedFocus;
	ImGui::SFML::ProcessEvent(event);

	return (ImGui::GetIO());
}

Gui::~Gui(void)
{
	ImGui::SFML::Shutdown();
}

boost::filesystem::path const	&Gui::getPathRessources() const {
	return (pathRessources_);
}

void			callbackExit(Gui &gui)
{
	gui.exit();
}

void			Gui::aState(void)
{
	WidgetExit wexit(*this, callbackExit);
	WidgetLobby lobby(*this);
	WidgetOption *optionSnake = nullptr;
	WidgetConnect optionConnect(*this);
	WidgetMassiveButton massiveButton(*this);
	sf::Event event;


	while (_win.isOpen() && !univers.isOpenGame_()) {
		ImGui::SFML::Update(_win, _deltaClock.restart());

		while (_win.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed && event.key.control) {
				switch (event.key.code) {
					case sf::Keyboard::A:
						univers.callbackAction(kCreateIA);
						break;
					case sf::Keyboard::B:
						univers.callbackAction(kBorderless);
						break;
					case sf::Keyboard::C:
						univers.callbackAction(kCreateClient);
						break;
					case sf::Keyboard::D:
						univers.callbackAction(kDeleteClient);
						break;
					case sf::Keyboard::E:
						univers.callbackAction(kConnect);
						break;
					case sf::Keyboard::R:
						univers.callbackAction(kSwitchReady);
						break;
					case sf::Keyboard::S:
						univers.callbackAction(kCreateServer);
						break;
					case sf::Keyboard::W:
						univers.callbackAction(kDeleteServer);
						break;
					case sf::Keyboard::X:
						univers.callbackAction(kStartGame);
						break;
					case sf::Keyboard::H:
						univers.callbackAction(kHostname);
						break;
					default:
						break;
				}
			}
			_processEvent(event);
			ImGui::SFML::ProcessEvent(event);
		}
		ImGui::SetNextWindowPos(positionByPercent(sf::Vector2<unsigned int>(0, 50)));
		ImGui::SetNextWindowSize(positionByPercent(sf::Vector2<unsigned int>(50, 50)));
		_chat.render();

		lobby.render();

		ImGui::SetNextWindowPos(positionByPercent(sf::Vector2<unsigned int>(95, 0)), 0, sf::Vector2f(0.5f, 0.5f));
		wexit.render();

		if (univers.getSnakeClient() && univers.getSnakeClient()->isConnect()) {
			if (!optionSnake)
				optionSnake = new WidgetOption(*this);
			ImGui::SetNextWindowPos(positionByPercent(sf::Vector2<unsigned int>(70, 50)));
			ImGui::SetNextWindowSize(positionByPercent(sf::Vector2<unsigned int>(30, 25)));
			optionSnake->render();
			ImGui::SetNextWindowPos(positionByPercent(sf::Vector2<unsigned int>(70, 75)));
			ImGui::SetNextWindowSize(positionByPercent(sf::Vector2<unsigned int>(30, 25)));
			optionConnect.render();
		}
		else {
			ImGui::SetNextWindowPos(positionByPercent(sf::Vector2<unsigned int>(70, 50)));
			ImGui::SetNextWindowSize(positionByPercent(sf::Vector2<unsigned int>(30, 50)));
			optionConnect.render();
		}

		ImGui::SetNextWindowPos(positionByPercent(sf::Vector2<unsigned int>(50, 50)));
		ImGui::SetNextWindowSize(positionByPercent(sf::Vector2<unsigned int>(20, 50)));
		massiveButton.render();

		_render();
	}
	if (optionSnake)
		delete optionSnake;
}

void				Gui::_render(void) {
	_win.clear();
	ImGui::SFML::Render(_win);
	_win.display();
}

void 				Gui::exit(void) {
	_win.close();
	univers.setExit(true);
}

sf::Vector2<unsigned int>	Gui::positionByPercent(sf::Vector2<unsigned int> const &percent) const {
	return (sf::Vector2<unsigned int>(_winSize.x * percent.x / 100,
										_winSize.y * percent.y / 100));
}

void					Gui::_processEvent(sf::Event const &event) {
	if (event.type == sf::Event::Resized)
		_winSize = sf::Vector2<unsigned int>(event.size.width, event.size.height);
	else if (event.type == sf::Event::Closed)
		exit();
}


void					Gui::beginColor(float const color) {
	assert(!Gui::_useColor);
	Gui::_useColor = true;
	ImGui::PushStyleColor(ImGuiCol_Button, static_cast<ImVec4>(ImColor::HSV(color, 0.7f, 0.7f)));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, static_cast<ImVec4>(ImColor::HSV(color, 0.8f, 0.8f)));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, static_cast<ImVec4>(ImColor::HSV(color, 0.9f, 0.9f)));
}
void					Gui::endColor() {
	ImGui::PopStyleColor(3);
	Gui::_useColor = false;
}


float const 				Gui::HUE_GREEN = 0.33f;
float const 				Gui::HUE_RED = 0.f;
float const 				Gui::HUE_PURPLE = 0.77f;
bool 						Gui::_useColor = false;

Gui::CoreConstructorException::~CoreConstructorException(void) noexcept{}
Gui::CoreConstructorException::CoreConstructorException(void) noexcept :
	_error("Error on Gui constructor") {}
Gui::CoreConstructorException::CoreConstructorException(std::string s) noexcept :
	_error(s) { }
Gui::CoreConstructorException::CoreConstructorException(Gui::CoreConstructorException const &src) noexcept :
	_error(src._error)
	{ _error = src._error; }
const char	*Gui::CoreConstructorException::what() const noexcept
	{ return (_error.c_str()); }

