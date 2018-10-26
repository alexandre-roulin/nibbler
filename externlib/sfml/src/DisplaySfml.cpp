#include <iostream>
#include "DisplaySfml.hpp"

DisplaySfml::DisplaySfml(char const *tileset,
						int tileSize,
						int width,
						int height,
						char const *windowName) :
    _exit(false),
	_tileSize(tileSize),
	_winTileSize(Vector2D<int>(width, height)),
    _winPixelSize(Vector2D<int>(width * tileSize, height * tileSize)),
    _win(sf::VideoMode(this->_winPixelSize.getX(),
                        this->_winPixelSize.getY()),
        windowName)
{
    static_cast<void>(tileset);
    this->_win.setFramerateLimit(60);
//    this->_error();


    if (!this->_tileset.loadFromFile(tileset))
        this->_error("Tileset cannot be load");
/*	if (!(this->_tileset = IMG_Load(tileset)))
		this->_error();
	this->_tilesetWidth = (this->_tileset->w / this->_tileSize);
*/
}

DisplaySfml::~DisplaySfml(void)
{
    this->_clean();
}

void DisplaySfml::_error(std::string s = std::string("Error"))
{
    this->_clean();
    throw(DisplaySfml::SfmlConstructorException(s));
}

void DisplaySfml::_clean(void)
{
    this->_win.close();
}

void DisplaySfml::render(void)
{
    this->_win.display();
}

/*
void			DisplaySfml::drawTile(int indexTile,
										int indexWidth, int indexHeight)
{
	SDL_Rect rectToDraw = {
		((indexWidth) * this->_tileSize),	((indexHeight) * this->_tileSize),
		(this->_tileSize),					(this->_tileSize) };
	SDL_Rect rectTilset = {
		(this->_tileSize) * (indexTile % this->_tilesetWidth),
		(this->_tileSize) * (indexTile / this->_tilesetWidth),
		(this->_tileSize),	(this->_tileSize)};
	SDL_BlitSurface(this->_tileset, &rectTilset, this->_rendererSurface, &rectToDraw);
}
void			DisplaySfml::drawTile(SDL_Surface *surface,
										int indexTile,
										int indexWidth, int indexHeight)
{
	SDL_Rect rectToDraw = {
		((indexWidth) * this->_tileSize),	((indexHeight) * this->_tileSize),
		(this->_tileSize),					(this->_tileSize) };
	SDL_Rect rectTilset = {
		(this->_tileSize) * (indexTile % this->_tilesetWidth),
		(this->_tileSize) * (indexTile / this->_tilesetWidth),
		(this->_tileSize),	(this->_tileSize)};
	SDL_BlitSurface(this->_tileset, &rectTilset, surface, &rectToDraw);
}

//
void			DisplaySfml::drawTile(int indexWidthTile, int indexHeightTile,
										int indexWidth, int indexHeight)
{
	SDL_Rect rectToDraw = {
		((indexWidth) * this->_tileSize),	((indexHeight) * this->_tileSize),
		(this->_tileSize),					(this->_tileSize) };
	SDL_Rect rectTilset = {
		(this->_tileSize) * indexWidthTile,	(this->_tileSize) * indexHeightTile,
		(this->_tileSize),					(this->_tileSize) };
	SDL_BlitSurface(this->_tileset, &rectTilset, this->_rendererSurface, &rectToDraw);
}
void			DisplaySfml::drawTile(SDL_Surface *surface,
										int indexWidthTile, int indexHeightTile,
										int indexWidth, int indexHeight)
{
	SDL_Rect rectToDraw = {
		((indexWidth) * this->_tileSize),	((indexHeight) * this->_tileSize),
		(this->_tileSize),					(this->_tileSize) };
	SDL_Rect rectTilset = {
		(this->_tileSize) * indexWidthTile,	(this->_tileSize) * indexHeightTile,
		(this->_tileSize),					(this->_tileSize) };
	SDL_BlitSurface(this->_tileset, &rectTilset, surface, &rectToDraw);
}

//
void			DisplaySfml::drawColorTile(int indexWidth, int indexHeight, int color)
{
	SDL_Rect r = {
		((indexWidth) * this->_tileSize),	((indexHeight) * this->_tileSize),
		(this->_tileSize),					(this->_tileSize) };
	SDL_FillRect(this->_rendererSurface, &r, color);
}
void			DisplaySfml::drawColorTile(SDL_Surface *surface,
										int indexWidth, int indexHeight, int color)
{
	SDL_Rect r = {
		((indexWidth) * this->_tileSize),	((indexHeight) * this->_tileSize),
		(this->_tileSize),					(this->_tileSize) };
	SDL_FillRect(surface, &r, color);
}


void			DisplaySfml::drawGrid(Grid<int> const &grid)
{
	for (int y = 0; y < this->_winTileSize.getY(); y++)
		for (int x = 0; x < this->_winTileSize.getX(); x++)
			this->drawTile(grid(x, y), x, y);
}
void			DisplaySfml::drawGrid(SDL_Surface *surface, Grid<int> const &grid)
{
	for (int y = 0; y < this->_winTileSize.getY(); y++)
		for (int x = 0; x < this->_winTileSize.getX(); x++)
			this->drawTile(surface, grid(x, y), x, y);
}

void		DisplaySfml::setBackground(Grid<int> const &grid)
{
	this->drawGrid(this->_background, grid);
}
*/
void			DisplaySfml::update(void)
{
    while (this->_win.pollEvent(this->_ev))
    {
        if (this->_ev.type == sf::Event::Closed)
            this->_exit = true;
    }
}

bool			DisplaySfml::exit(void) const
{
    return (this->_exit);
}

DisplaySfml::SfmlConstructorException::~SfmlConstructorException(void) throw(){}
DisplaySfml::SfmlConstructorException::SfmlConstructorException(void) throw() :
	_error("Error on Sfml constructor") {}
DisplaySfml::SfmlConstructorException::SfmlConstructorException(std::string s) throw() :
	_error(s) { }
DisplaySfml::SfmlConstructorException::SfmlConstructorException(DisplaySfml::SfmlConstructorException const &src) throw() :
	_error(src._error)
	{ this->_error = src._error; }
const char	*DisplaySfml::SfmlConstructorException::what() const throw()
	{ return (this->_error.c_str()); }
