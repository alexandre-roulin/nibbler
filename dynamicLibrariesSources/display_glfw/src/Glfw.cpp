#include "Glfw.hpp"
#include <iostream>
Glfw::Glfw(std::string const &name, uint16_t width, uint16_t height) :
    cursor_(true) {
    std::cout << "Glfw" << std::endl;
    glfwInit();
    std::cout << "glfwInit" << std::endl;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    std::cout << "glfwWindowHint" << std::endl;
    if (!(window_ = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr))) {
		std::cout << "ERR" << std::endl;
        clean_();
		std::cout << "ERR" << std::endl;
		throw (Glfw::ConstructorException("GlfwConstructorException: window was not created"));
    }
    std::cout << "glfwCreateWindow" << std::endl;

    glfwMakeContextCurrent(window_);
    std::cout << "glfwMakeContextCurrent" << std::endl;
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
    glfwSwapInterval(0);
}

Glfw::~Glfw() {
    clean_();
}

void    Glfw::clean_() {
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwTerminate();
}

void            Glfw::update() {
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window_, true);
    if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (cursor_)
            glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        cursor_ = !cursor_;
    }

}

void            Glfw::render() {
    glfwSwapBuffers(window_);
    glfwPollEvents();
}

bool            Glfw::exit() const {
    return static_cast<bool>(glfwWindowShouldClose(window_));
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