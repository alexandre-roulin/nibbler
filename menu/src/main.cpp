#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "IDisplay.hpp"
#include <dlfcn.h>
#include <iostream>

bool        dlError(void)
{
    std::cerr << "Error : " << dlerror() << std::endl;
    return (false);
}
int main(int argc, char **argv)
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "ImGui + SFML = <3");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
	bool		my_tool_active = true;

	static_cast<void>(argc);
    static_cast<void>(argv);

    void *dlHandle;

    if (!(dlHandle = dlopen(argv[1], RTLD_LAZY | RTLD_LOCAL)))
        return (dlError());

    IDisplay    *(*newDisplay)(char const *, int, int, int, char const *);
    void        (*deleteDisplay)(IDisplay *);
    if (!(newDisplay = (IDisplay *(*)(char const *, int, int, int, char const *))(dlsym(dlHandle, "newDisplay"))))
        return (dlError());
    if (!(deleteDisplay = (void (*)(IDisplay *))(dlsym(dlHandle, "deleteDisplay"))))
        return (dlError());

    IDisplay    *display = newDisplay("../commun/tileset1.png", 32, 30, 30, "lol");

    Grid<int> grid(30, 30);

	grid.fill(0);
	grid.setBorder(1);

	display->setBackground(grid);


    while (window.isOpen() && !display->exit())
	{
        sf::Event event;
        while (window.pollEvent(event))
		{
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
			{
                window.close();
            }
        }

        ImGui::SFML::Update(window);

		ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{
		    if (ImGui::BeginMenu("File"))
		    {
		        if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
		        if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
		        if (ImGui::MenuItem("Close", "Ctrl+W"))  { my_tool_active = false; }
		        ImGui::EndMenu();
		    }
		    ImGui::EndMenuBar();
		}

		// Edit a color (stored as ~4 floats)

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

        window.clear();
        ImGui::SFML::Render(window);
        window.display();

		display->update();
		std::cout << display->getDirection() << std::endl;
		display->drawTileGrid(0, 0, 0);
		display->drawTileGrid(1, 1, 1);
		display->drawTileGrid(2, 2, 2);
		display->drawTilePixel(12, 2*32+16, 2*32+16);
		display->render();
    }

	deleteDisplay(display);
    ImGui::SFML::Shutdown();
}
