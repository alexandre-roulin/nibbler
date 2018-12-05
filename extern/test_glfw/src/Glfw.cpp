#include "Glfw.hpp"
#include <iostream>
Glfw::Glfw(std::string const &name, uint16_t width, uint16_t height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    if (!(window_ = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr))) {
        clean_();
        throw (Glfw::ConstructorException("GlfwConstructorException: window was not created"));
    }
    glfwMakeContextCurrent(window_);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
}

Glfw::~Glfw() {
    clean_();
}

void    Glfw::clean_() {
    glfwTerminate();
}

void            Glfw::update() {
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window_, true);
}

void            Glfw::render() {
    glfwSwapBuffers(window_);
    glfwPollEvents();
}

bool            Glfw::exit() const {
    return (glfwWindowShouldClose(window_));
}

GLFWwindow     *Glfw::getWindow() const {
    return (window_);
}

Glfw::ConstructorException::~ConstructorException(void) throw(){}
Glfw::ConstructorException::ConstructorException(void) throw() :
        _error("Error on Glfw constructor") {}
Glfw::ConstructorException::ConstructorException(std::string s) throw() :
        _error(s) { }
Glfw::ConstructorException::ConstructorException(Glfw::ConstructorException const &src) throw() :
        _error(src._error)
{ this->_error = src._error; }
const char	*Glfw::ConstructorException::what() const throw()
{ return (this->_error.c_str()); }