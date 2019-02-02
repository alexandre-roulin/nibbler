#pragma once

#include <memory>
#include <string>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdint>

enum class eKeyState { kNone, kDown, kPress, kRelease };

inline bool operator!(eKeyState keyState) {
    return (keyState == eKeyState::kNone);
}

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

protected:
    bool                        cursor_;

private:
    GLFWwindow                  *window_;

    std::map<int, eKeyState> keyCurrent_;
    std::map<int, eKeyState> keyPast_;

    void            clean_();
    eKeyState       getKeyState(int key) const;

    static eKeyState       getKeyStateOf_(int key, std::map<int, eKeyState> const &keyState_);
    static void            callbackKey_(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void            callbackError_(int error, const char* errorMessage);
    static std::map<GLFWwindow*, Glfw&>     glfwByWindow_;

    Glfw(Glfw const &shader) = delete;
    Glfw &operator=(Glfw const &shader) = delete;
};