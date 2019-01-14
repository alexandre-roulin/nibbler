#pragma once

#include <exception>
#include <glm/glm.hpp>
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
#include "Grid.tpp"
#include "Skybox.hpp"

#define DISPLAY_GLFW_WIN_WIDTH 1024
#define DISPLAY_GLFW_WIN_HEIGHT 720

#ifdef _WIN32
    #define DISPLAY_GLFW_SLASH "\\"
#else
    #define DISPLAY_GLFW_SLASH "/"
#endif

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

	void render() override;
    void setFrameTime(float);
    void update(float deltaTime = 0.16f);
	void update() override;
	bool        exit() const override;
	eDirection getDirection() const override;
	void		drawGrid(Grid< eSprite > const &grid) override;
	void		setBackground(Grid< eSprite > const &grid) override;

	DisplayGlfw &operator=(DisplayGlfw const &rhs) = delete;
	DisplayGlfw(DisplayGlfw const &src) = delete;
	DisplayGlfw() = delete;

private:
    eDirection          direction_;
    float				frameTime_;
    int                 tileSize_;
    Vector2D<int> const winTileSize_;
    Vector2D<int> const winPixelSize_;

	Grid< eSprite >		tileBackground_;
    Grid< ActModel >	background_;

	float				deltaTime_;

    std::string         pathRoot_;
    std::string         pathModel_;
	std::string         pathBlock_;
	std::string         pathGround_;
	std::string         pathWall_;
	std::string			pathDirectorySkyBox_;
	std::string			pathShaderBasic_;
    std::string			pathShaderSkyBox_;
    std::vector< std::string >		pathSkyBox_;

	Shader							shader_;
	std::unique_ptr< Skybox >		skybox_;
	Model							snake_;
	Model							block_;
	Model							ground_;
	Model							wall_;

	ActModel						asnake_;
	std::unique_ptr< ActModel[] > 	ablock_;

    Camera							camera_;

	glm::mat4						projection_;
	glm::mat4						view_;
	glm::mat4						model_;


    void                error_(std::string const &s = std::string("Error"));
    void                clean_();
    void                getPath_();
    void				loadSkyBox_();

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