#pragma once

#include <exception>
#include "IDisplay.hpp"
#include "Vector2D.tpp"

class DisplayGlfw {
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

        std::string _error;
    };

    DisplayGlfw(char const *tileset,
                int tile,
                int width,
                int height,
                char const *windowName);

    virtual ~DisplayGlfw(void);

    bool exit(void) const;
    void render(void);
    void update(void);
    eDirection getDirection(void) const;

private:
    bool _exit;
    eDirection _direction;
    int _tileSize;
    Vector2D<int> const _winTileSize;
    Vector2D<int> const _winPixelSize;

    int _tilesetWidth;
    void _error(std::string);
    void _clean(void);


    DisplayGlfw &operator=(DisplayGlfw const &rhs);

    DisplayGlfw(DisplayGlfw const &src);

    DisplayGlfw(void);
};

//extern "C" {
//IDisplay *newDisplay(char const *tileset,
//                     int tileSize,
//                     int width,
//                     int height,
//                     char const *windowName);
//void deleteDisplay(IDisplay *display);
//}