#pragma once

#include <memory>
#include <string>
#include <map>
#include <list>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include "KeyStateManager.hpp"

class Glfw : public KeyStateManager {
public:
    class ConstructorException : public std::exception {
    public:
        ConstructorException(void) throw();
        ConstructorException(std::string) throw();
        virtual const char* what() const throw();
        ~ConstructorException(void) throw();
        ConstructorException(ConstructorException const &src) throw();
    private:
        ConstructorException &operator=(ConstructorException const &rhs) throw();
        std::string			_error;
    };

    Glfw(std::string const &name, uint16_t width, uint16_t height);
    ~Glfw();

    void            update() override;
    void            render();
    bool            exit() const;

    GLFWwindow     *getWindow() const;

protected:
    bool                        cursor_;

private:
    GLFWwindow                  *window_;

    void            clean_();

    static void            callbackKey_(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void            callbackError_(int error, const char* errorMessage);
    static std::map<GLFWwindow*, Glfw&>     glfwByWindow_;

    Glfw(Glfw const &shader) = delete;
    Glfw &operator=(Glfw const &shader) = delete;
};