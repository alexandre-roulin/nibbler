#pragma once

#include <exception>
#include "IDisplay.hpp"
#include "Vector2D.tpp"
#include "Glfw.hpp"
#include "Shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>

#include "Glfw.hpp"
#include "Model.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "ActModel.hpp"
#include <fstream>
#include <map>
#include "Grid.tpp"

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
        GlfwConstructorException(void) throw();

        GlfwConstructorException(std::string) throw();

        virtual const char *what() const throw();

        ~GlfwConstructorException(void) throw();

        GlfwConstructorException(GlfwConstructorException const &src) throw();

    private:
        GlfwConstructorException &
        operator=(GlfwConstructorException const &rhs) throw();

        std::string error_;
    };

    DisplayGlfw(int width,
                int height,
                char const *windowName);

    virtual ~DisplayGlfw(void);

	void render(void);
    void setFrameTime(float);
    void update(float deltaTime = 0.16f);
	void update();
	bool        exit(void) const;
	eDirection getDirection(void) const;
	//void		drawGrid(Grid<int> const &grid);
	void		drawGrid(Grid< eSprite > const &grid);
	void		setBackground(Grid< eSprite > const &grid);


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
    std::string			pathSkyBox_[6];

    unsigned int					textureSkyBox_;
	unsigned int					skyboxVAO_;
	unsigned int					skyboxVBO_;

	Shader							shader_;
	Shader							shaderSkyBox_;
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


	DisplayGlfw &operator=(DisplayGlfw const &rhs) = delete;
    DisplayGlfw(DisplayGlfw const &src) = delete;
    DisplayGlfw(void) = delete;
};

extern "C" {
IDisplay *newDisplay(int width,
                     int height,
                     char const *windowName);
void deleteDisplay(IDisplay *display);
}