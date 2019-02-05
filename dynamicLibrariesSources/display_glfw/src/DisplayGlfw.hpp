#pragma once

#include <exception>
#include <glm/glm.hpp>
#include <boost/filesystem.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <map>

#include "IDisplay.hpp"
#include "Vector2D.tpp"
#include "Glfw.hpp"
#include "Shader.hpp"
#include "Glfw.hpp"
#include "Model.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "ActModel.hpp"
#include "MutantGrid.tpp"
#include "Particle.hpp"
#include "Material.hpp"
#include "Light.hpp"

#define DISPLAY_GLFW_WIN_WIDTH 1024
#define DISPLAY_GLFW_WIN_HEIGHT 720
#define FLAG_LINE 2

class DisplayGlfw : public Glfw, public IDisplay {
public:
    class GlfwConstructorException : public std::exception {
    public:
        GlfwConstructorException() noexcept;

		explicit GlfwConstructorException(std::string) noexcept;

		const char *what() const noexcept override;

        ~GlfwConstructorException() noexcept override;

        GlfwConstructorException(GlfwConstructorException const &src) noexcept;

    private:
        GlfwConstructorException &
        operator=(GlfwConstructorException const &rhs) noexcept;

        std::string error_;
    };

    DisplayGlfw(int width,
                int height,
                char const *windowName);

	~DisplayGlfw() override;

	void render(float currentDelayFrame, float maxDelayFrame) override;
    void update(float deltaTime) override;
	bool        exit() const override;
	eDirection getDirection() const override;
	void		drawGrid(MutantGrid< eSprite > const &grid) override;
	void		setBackground(MutantGrid< eSprite > const &grid) override;
	void		registerCallbackAction(std::function<void(eAction)>) override;

	DisplayGlfw &operator=(DisplayGlfw const &rhs) = delete;
	DisplayGlfw(DisplayGlfw const &src) = delete;
	DisplayGlfw() = delete;

private:
	std::bitset<16>					flag_;
	boost::filesystem::path			pathRoot_;
    eDirection          			direction_;
	float							currentTimer_;
	float							maxTimer_;
	float							refreshMaxTimer_;
    int                 			tileSize_;
    Vector2D<int> const 			winTileSize_;
    Vector2D<int> const 			winPixelSize_;

	MutantGrid< eSprite >			tileBackground_;
    MutantGrid< ActModel >			background_;
	MutantGrid< eSprite >			tileGrid_;
	MutantGrid< ActModel >			grid_;

	float							deltaTime_;

	eSprite 						yourSnakeSprite;
	int 							yourSnakeX;
	int 							yourSnakeY;
	Shader							shader_;
	Shader							shaderMultiple_;
	Model							block_;
	Model							modelGrass_;
	Model							modelWall_;
	Model							appleModel_;
	Model							modelSphere_;
	Model							modelHead_;
	Particle						*particuleBackground_;
	Particle						*particuleBackgroundOutline_;
	Light							light_;
	std::vector<Camera>				camera_;
	int								indexActiveCamera_;
    ActModel						actBlock_;

	glm::mat4						projection_;
	glm::mat4						view_;
	glm::mat4						model_;

	std::function<void(eAction)>	_callback;


	void            	    error_(std::string const &s = std::string("Error"));
    void            	    clean_();
	void					activeNextCamera_();
	Camera					&getActiveCamera_();

	void					drawGridCase_(eSprite sprite, int x, int y);
	void					drawGridCaseBody_(int x, int y);
	void					interpolateGridCase_(int x, int y);
	void					renderLine_(ActModel const &model);
	void					renderAtDirectionFrom(ActModel const &from, eSprite at);

	void					drawHelpLineSnake_();


	std::map< eSprite, Material >		materialMap_;
	void constructMaterialMap_();

	static float				lastX_;
	static float				lastY_;
	static float				offsetX_;
	static float				offsetY_;
	static bool					firstMouse_;
	static bool					mouseCallbackCalled_;
	static void					mouseCallback_(GLFWwindow* window, double xpos, double ypos);
};

extern "C" {
IDisplay *newDisplay(int width,
                     int height,
                     char const *windowName);
void deleteDisplay(IDisplay *display);
}