#pragma once

#include <memory>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdint>

class Glfw {
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

    void            update();
    void            render();
    bool            exit() const;

    GLFWwindow     *getWindow() const;

private:
    GLFWwindow     *window_;

    void            clean_();

    Glfw(Glfw const &shader) = delete;
    Glfw &operator=(Glfw const &shader) = delete;
};