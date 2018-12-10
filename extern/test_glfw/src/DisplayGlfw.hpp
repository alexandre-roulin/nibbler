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
#include <fstream>


#define DISPLAY_GLFW_WIN_WIDTH 1024
#define DISPLAY_GLFW_WIN_HEIGHT 720

#ifdef _WIN32
    #define DISPLAY_GLFW_SLASH "\\"
#else
    #define DISPLAY_GLFW_SLASH "/"
#endif

class DisplayGlfw : public Glfw {
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

    DisplayGlfw(char const *tileset,
                int tile,
                int width,
                int height,
                char const *windowName);

    virtual ~DisplayGlfw(void);

    void render(void);
    void update(float deltaTime = 0.16f);
    eDirection getDirection(void) const;

private:
    eDirection          direction_;
    int                 tileSize_;
    Vector2D<int> const winTileSize_;
    Vector2D<int> const winPixelSize_;

	float				deltaTime_;

    std::string         pathRoot_;
    std::string         pathModel_;
    std::string         pathBlock_;
    std::string         pathShaderVert_;
    std::string         pathShaderFrag_;

    Shader              shader_;
	Model               snake_;
	Model               block_;

    Camera				camera_;

	glm::mat4			projection_;
	glm::mat4			view_;
	glm::mat4			model_;


    void                error_(std::string const &s = std::string("Error"));
    void                clean_();
    void                getPath_();

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

//extern "C" {
//IDisplay *newDisplay(char const *tileset,
//                     int tileSize,
//                     int width,
//                     int height,
//                     char const *windowName);
//void deleteDisplay(IDisplay *display);
//}