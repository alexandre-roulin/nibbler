#include "Core.hpp"
#include "WidgetExit.hpp"
#include "WidgetSnake.hpp"

Core::Core(void) :
_winSize(sf::Vector2<unsigned int>(900, 800)),
_win(sf::VideoMode(this->_winSize.x, this->_winSize.y), "Project Sanke"),
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

Core::~Core(void)
{
	ImGui::SFML::Shutdown();
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


void			callbackExit(void *ptr)
{
	static_cast<Core *>(ptr)->exit();
}

void			Core::aState(void)
{
	WidgetExit wexit(&callbackExit, this);
	WidgetSnake snake;

	while (this->_win.isOpen())
	{
		sf::Event event;
		while (this->_win.pollEvent(event))
		{
			this->_processEvent(event);
			ImGui::SFML::ProcessEvent(event);
		}

		ImGui::SFML::Update(this->_win, this->_deltaClock.restart());

		ImGui::SetNextWindowPos(this->positionByPercent(sf::Vector2<unsigned int>(0, 50)));
		ImGui::SetNextWindowSize(this->positionByPercent(sf::Vector2<unsigned int>(100, 50)));
		this->_chat.render();


		ImGui::SetNextWindowPos(this->positionByPercent(sf::Vector2<unsigned int>(0, 5)));
		ImGui::SetNextWindowSize(this->positionByPercent(sf::Vector2<unsigned int>(100 / (MAX_SNAKE / 2), 45 / 2)));
		snake.render();


		static double last_time = -1.0;
		double time = ImGui::GetTime();
		if (time - last_time >= 0.90f && !ImGui::GetIO().KeyCtrl)
		{
			const char* random_words[] = { "system", "info", "warning", "error", "fatal", "notice", "log" };
			this->_chat.addLog("[%s] Hello, time is %.1f, frame count is %d\n", random_words[rand() % IM_ARRAYSIZE(random_words)], time, ImGui::GetFrameCount());
			last_time = time;
		}

		ImGui::SetNextWindowPos(this->positionByPercent(sf::Vector2<unsigned int>(95, 0)), 0, sf::Vector2f(0.5f, 0.5f));
		wexit.render();

		this->_render();
	}
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

sf::Vector2<unsigned int>	Core::positionByPercent(sf::Vector2<unsigned int> const &percent)
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
