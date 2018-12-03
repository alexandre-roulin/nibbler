#include <nibbler.hpp>
#include "DisplayGlfw.hpp"

//IDisplay *newDisplay(char const *tileset,
//                     int tileSize,
//                     int width,
//                     int height,
//                     char const *windowName) {
//    return (new DisplayGlfw(tileset, tileSize, width, height, windowName));
//}

void deleteDisplay(IDisplay *display) {
    delete display;
}

DisplayGlfw::DisplayGlfw(char const *tileset,
                         int tileSize,
                         int width,
                         int height,
                         char const *windowName) :
        _exit(false),
        _direction(NORTH),
        _tileSize(tileSize),
        _winTileSize(Vector2D<int>(width, height)),
        _winPixelSize(Vector2D<int>(width * tileSize, height * tileSize)) {
    //this->_win.setFramerateLimit(60);

    //if (!this->_tileset.loadFromFile(tileset))
    //    this->_error("Tileset cannot be load");
    //if (!this->_textureBackground.create(this->_winPixelSize.getX(),
    //                                     this->_winPixelSize.getY()))
    //    this->_error("Error during creation of Texture background");
    //this->_tilesetWidth = (this->_tileset.getSize().x / this->_tileSize);
    //this->_win.clear();
    //this->_textureBackground.clear();
    //this->_win.display();
}

DisplayGlfw::~DisplayGlfw(void) {
    this->_clean();
}

void DisplayGlfw::_error(std::string s = std::string("Error")) {
    this->_clean();
    throw (DisplayGlfw::GlfwConstructorException(s));
}

void DisplayGlfw::_clean(void) {
    //this->_win.close();
}

void DisplayGlfw::render(void) {
    //this->_win.display();
    //this->_win.draw(this->_spriteBackground);
}

/*
**####################ID_TILE
*/

void DisplayGlfw::update(void) {
}

eDirection DisplayGlfw::getDirection(void) const {
    return (this->_direction);
}

bool DisplayGlfw::exit(void) const {
    return (this->_exit);
}

DisplayGlfw::GlfwConstructorException::~GlfwConstructorException(
        void) throw() {}

DisplayGlfw::GlfwConstructorException::GlfwConstructorException(void) throw() :
        _error("Error on Glfw constructor") {}

DisplayGlfw::GlfwConstructorException::GlfwConstructorException(
        std::string s) throw() :
        _error(s) {}

DisplayGlfw::GlfwConstructorException::GlfwConstructorException(
        DisplayGlfw::GlfwConstructorException const &src) throw() :
        _error(src._error) { this->_error = src._error; }

const char *
DisplayGlfw::GlfwConstructorException::what() const throw() { return (this->_error.c_str()); }
