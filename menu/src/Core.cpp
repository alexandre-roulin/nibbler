#include "Core.hpp"

Core::Core(void) :
_win(sf::VideoMode(640, 480), "ImGui + SFML = <3")

{
	if (!this->_background.loadFromFile("ecran_titre.png"))
		(throw(Core::CoreConstructorException("Cannot load background")));
	this->_win.setFramerateLimit(60);
	ImGui::SFML::Init(this->_win);
}

bool			Core::TitleScreen(void)
{
	while (this->_win.isOpen())
	{
		sf::Event event;
		while (this->_win.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
			{
				this->_win.close();
			}
		}

		ImGui::Image(this->_background);

		ImGui::End();

		this->_win.clear();
		ImGui::SFML::Render(this->_win);
		this->_win.display();
	}
	return (true);
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
			{
				this->_win.close();
			}
		}

		ImGui::SFML::Update(this->_win, this->_deltaClock.restart());
		ImGui::Begin("My First Tool", &this->_my_tool_active, ImGuiWindowFlags_MenuBar);

		ImGui::Image(this->_background);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
				if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
				if (ImGui::MenuItem("Close", "Ctrl+W"))  { this->_my_tool_active = false; }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// Plot some values
		const float my_values[] = { 0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f };
		ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

		// Display contents in a scrolling region
		ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff");
		ImGui::BeginChild("Scrolling");
		for (int n = 0; n < 50; n++)
			ImGui::Text("%04d: Some text", n);
		ImGui::EndChild();
		ImGui::End();

		this->_win.clear();
		ImGui::SFML::Render(this->_win);
		this->_win.display();
	}
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
