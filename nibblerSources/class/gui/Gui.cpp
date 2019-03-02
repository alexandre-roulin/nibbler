#include <cores/Univers.hpp>
#include "Gui.hpp"
#include "widget/WidgetExit.hpp"
#include "widget/WidgetSnake.hpp"
#include "widget/WidgetLobby.hpp"
#include "gui/widget/WidgetOption.hpp"
#include "gui/widget/WidgetSettingGame.hpp"
#include "gui/widget/WidgetConnect.hpp"
#include "gui/widget/WidgetMassiveButton.hpp"
#include "gui/widget/WidgetServerPannel.hpp"
#include "gui/widget/WidgetEtat.hpp"
#include "cores/Test.hpp"

Gui::Gui(Univers &univers) :
univers(univers),
pathRessources_(boost::filesystem::path(NIBBLER_ROOT_PROJECT_PATH) / "ressources/"),
_winSize(sf::Vector2<unsigned int>(1000, 900)),
_win(sf::VideoMode(_winSize.x, _winSize.y), "Project Sanke"),
_io(_createContext()),
_chat(*this),
_mapSize(sf::Vector2<int>(20, 20))
{
	boost::filesystem::path input(GUI_INPUT_DIRECTORY);
	boost::filesystem::create_directory(input);

	_io.IniFilename = NULL;


	time_t t;
	tm tm;
	t = std::time(nullptr);
	tm = *std::localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(&tm, "%d-%m_%H-%M-%S");
	input_.open((input / "input_").generic_string() + oss.str());
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
	input_.close();
}

boost::filesystem::path const	&Gui::getPathRessources() const {
	return (pathRessources_);
}

void			callbackExit(Gui &gui)
{
	gui.exit();
}

void			callbackTest(void *vUnivers, std::string const &input) {
	Univers *univers = reinterpret_cast<Univers*>(vUnivers);

	if (!vUnivers)
		return ;
	else if (input == "A")
		univers->createBobby();
	else if (input == "B")
		univers->switchBorderless();
	else if (input == "C")
		univers->createClient();
	else if (input == "D")
		univers->deleteClient();
	else if (input == "E")
		univers->connect();
	else if (input == "R")
		univers->switchReady();
	else if (input == "S")
		univers->createServer();
	else if (input == "W")
		univers->deleteServer();
	else if (input == "X")
		univers->sendOpenGameToServer();
	else if (input == "H")
		univers->sendHostname();
}

void			Gui::aState(void)
{
	WidgetExit wexit(*this, callbackExit);
	WidgetLobby lobby(*this);
	WidgetOption *optionSnake = nullptr;
	WidgetServerPannel serverPannel(*this);
	WidgetConnect wOptionConnect(*this);
	WidgetEtat wEtat(*this);
	WidgetMassiveButton massiveButton(*this);
	sf::Event event;

	Test::getInstance().setInputCallback(callbackTest, reinterpret_cast<void*>(&univers));

	while (Test::getInstance().needUpdate() || (_win.isOpen() && !univers.isOpenGame_())) {

		WidgetSettingGame settings(*this);

		ImGui::SFML::Update(_win, _deltaClock.restart());
		Test::getInstance().update();
		while (_win.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed && event.key.control) {
				switch (event.key.code) {
					case sf::Keyboard::A:
						univers.createBobby();
						input_ << "A";
						Test::getInstance().writeInput("A");
						break;
					case sf::Keyboard::B:
						univers.switchBorderless();
						input_ << "B";
						Test::getInstance().writeInput("B");
						break;
					case sf::Keyboard::C:
						univers.createClient();
						input_ << "C";
						Test::getInstance().writeInput("C");
						break;
					case sf::Keyboard::D:
						univers.deleteClient();
						input_ << "D";
						Test::getInstance().writeInput("D");
						break;
					case sf::Keyboard::E:
						univers.connect();
						input_ << "E";
						Test::getInstance().writeInput("E");
						break;
					case sf::Keyboard::R:
						univers.switchReady();
						input_ << "R";
						Test::getInstance().writeInput("R");
						break;
					case sf::Keyboard::S:
						univers.createServer();
						input_ << "S";
						Test::getInstance().writeInput("S");
						break;
					case sf::Keyboard::W:
						univers.deleteServer();
						input_ << "W";
						Test::getInstance().writeInput("W");
						break;
					case sf::Keyboard::X:
						univers.sendOpenGameToServer();
						input_ << "X";
						Test::getInstance().writeInput("X");
						break;
					case sf::Keyboard::H:
						univers.sendHostname();
						input_ << "H";
						Test::getInstance().writeInput("H");
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
		_chat.render(true);

		lobby.render(true);

		ImGui::SetNextWindowPos(positionByPercent(sf::Vector2<unsigned int>(95, 0)), 0, sf::Vector2f(0.5f, 0.5f));
		wexit.render(true);
		SnakeClient::boost_shared_ptr ptr(univers.getSnakeClient().lock());

		ImGui::SetNextWindowPos(positionByPercent(sf::Vector2<unsigned int>(50, 50)));
		ImGui::SetNextWindowSize(positionByPercent(sf::Vector2<unsigned int>(20, 15)));
		settings.render((ptr && ptr->isOpen()));

		if (ptr && ptr->isOpen()) {
			if (!optionSnake)
				optionSnake = new WidgetOption(*this);

			ImGui::SetNextWindowPos(positionByPercent(sf::Vector2<unsigned int>(70, 50)));
			ImGui::SetNextWindowSize(positionByPercent(sf::Vector2<unsigned int>(30, 15)));
			optionSnake->render(true);
		}
		else if (ptr) {

			if (!optionSnake)
				optionSnake = new WidgetOption(*this);

			ImGui::SetNextWindowPos(positionByPercent(sf::Vector2<unsigned int>(70, 50)));
			ImGui::SetNextWindowSize(positionByPercent(sf::Vector2<unsigned int>(30, 15)));
			optionSnake->render(true);

		}

		ImGui::SetNextWindowPos(positionByPercent(sf::Vector2<unsigned int>(70, 65)));
		ImGui::SetNextWindowSize(positionByPercent(sf::Vector2<unsigned int>(30, 20)));
		wOptionConnect.render(true);
		ImGui::SetNextWindowPos(positionByPercent(sf::Vector2<unsigned int>(70, 85)));
		ImGui::SetNextWindowSize(positionByPercent(sf::Vector2<unsigned int>(30, 15)));
		wEtat.render(true);

		if (univers.isServer()) {
			ImGui::SetNextWindowPos(positionByPercent(sf::Vector2<unsigned int>(50, 65)));
			ImGui::SetNextWindowSize(positionByPercent(sf::Vector2<unsigned int>(20, 20)));
			serverPannel.render(true);
		}

		ImGui::SetNextWindowPos(positionByPercent(sf::Vector2<unsigned int>(50, 85)));
		ImGui::SetNextWindowSize(positionByPercent(sf::Vector2<unsigned int>(20, 15)));
		massiveButton.render(true);

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


void					Gui::beginHueColor_(float color, float power) {
	ImGui::PushStyleColor(ImGuiCol_Button, static_cast<ImVec4>(ImColor::HSV(color, 0.7f * power, 0.7f * power)));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, static_cast<ImVec4>(ImColor::HSV(color, 0.8f * power, 0.8f * power)));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, static_cast<ImVec4>(ImColor::HSV(color, 0.9f * power, 0.9f * power)));
}
void					Gui::beginColor(Gui::eColor color, Gui::eColor power) {
	assert(!Gui::_useColor);
	Gui::_useColor = true;
	if (color == Gui::eColor::kGrey) {
		ImGui::PushStyleColor(ImGuiCol_Button, static_cast<ImVec4>(ImColor::HSV(0.f, 0.f, 0.5f * Gui::mapEColor_.at(power))));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, static_cast<ImVec4>(ImColor::HSV(0.f, 0.0f, 0.5f * Gui::mapEColor_.at(power))));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, static_cast<ImVec4>(ImColor::HSV(0.f, 0.00f, 0.65f * Gui::mapEColor_.at(power))));
	} else
		beginHueColor_(Gui::mapEColor_.at(color), Gui::mapEColor_.at(power));
}
void					Gui::endColor() {
	ImGui::PopStyleColor(3);
	Gui::_useColor = false;
}

std::map< Gui::eColor, float > const Gui::mapEColor_ = { { eColor::kGrey, 0.f },
														 { eColor::kRed, 0.f },
														 { eColor::kGreen, 0.33f },
														 { eColor::kPurple, 0.77f },
														 { eColor::kHard, 1.f },
														 { eColor::kMedium, .75f },
														 { eColor::kSoft, .5f },
														 { eColor::kLight, .33f }};

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

