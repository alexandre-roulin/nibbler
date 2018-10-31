#include "Core.hpp"

Core::Core(void) :
_win(sf::VideoMode(900, 800), "ImGui + SFML = <3"),
_io(this->_createContext())
{
	if (!this->_background.loadFromFile("ecran_titre.png"))
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

bool			Core::titleScreen(void)
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
		ImGui::Begin("My First Tool", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove);
		ImGui::Image(this->_background);
		ImGui::End();
		this->_render();
	}
	return (true);
}

void			Core::demo(void)
{
	while (this->_win.isOpen())
	{
		sf::Event event;
		while (this->_win.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
				this->_win.close();
		}

		ImGui::SFML::Update(this->_win, this->_deltaClock.restart());
		ImGui::ShowDemoWindow();
		this->_render();
	}
}

void			Core::aState(void)
{
	while (this->_win.isOpen())
	{
		sf::Event event;
		while (this->_win.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
				this->_win.close();
		}

		ImGui::SFML::Update(this->_win, this->_deltaClock.restart());
		this->_chat.render();
		static double last_time = -1.0;
	    double time = ImGui::GetTime();
	    if (time - last_time >= 0.20f && !ImGui::GetIO().KeyCtrl)
	    {
	        const char* random_words[] = { "system", "info", "warning", "error", "fatal", "notice", "log" };
	        this->_chat.addLog("[%s] Hello, time is %.1f, frame count is %d\n", random_words[rand() % IM_ARRAYSIZE(random_words)], time, ImGui::GetFrameCount());
	        last_time = time;
	    }

		this->_render();
	}
}

void				Core::_render(void)
{
	this->_win.clear();
	ImGui::SFML::Render(this->_win);
	this->_win.display();
}

Core::~Core(void)
{
	ImGui::SFML::Shutdown();
}


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
