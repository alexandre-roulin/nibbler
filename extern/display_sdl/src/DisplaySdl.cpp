
#include <DisplaySdl.hpp>

#include "DisplaySdl.hpp"
#include "../include/DisplaySdl.hpp"
#include "Display2D.hpp"

IDisplay			*newDisplay(int width,
						int height,
						char const *windowName)
{
	return (new DisplaySdl(width, height, windowName));
}

void				deleteDisplay(IDisplay *display)
{
	delete display;
}

DisplaySdl::DisplaySdl(int width,
						int height,
						char const *windowName) :
    _exit(false),
	_direction(kNorth),
	_tileSize(DISPLAY_DEFAULT_TILE_SIZE),
	_winTileSize(Vector2D<int>(width, height)),
    _winPixelSize(Vector2D<int>(width * _tileSize, height * _tileSize)),
    _win(NULL),
    _rendererSurface(NULL),
    _rendererTexture(NULL),
    _renderer(NULL)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || IMG_Init(IMG_INIT_PNG) < 0)
        this->_error();

	std::string pathFile = __FILE__;
	std::string pathRoot = pathFile.substr(0, pathFile.rfind(DISPLAY_SLASH));
	pathRoot = pathFile.substr(0, pathRoot.rfind(DISPLAY_SLASH));

	if (!(this->_tileset = IMG_Load((pathRoot + DISPLAY_SLASH + "snake_tileset.png").c_str())))
		this->_error();
	this->_tilesetWidth = (this->_tileset->w / this->_tileSize);
    if (!(this->_win = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, this->_winPixelSize.getX(), this->_winPixelSize.getY(),
            SDL_WINDOW_SHOWN)))
        this->_error();

    if (!(this->_renderer = SDL_CreateRenderer(this->_win, -1, SDL_RENDERER_ACCELERATED)))
        this->_error();
    if (!(this->_rendererSurface = DisplaySdl::_SdlSurface(this->_winPixelSize.getX(), this->_winPixelSize.getY())))
        this->_error();
	if (!(this->_background = DisplaySdl::_SdlSurface(this->_winPixelSize.getX(), this->_winPixelSize.getY())))
		this->_error();
	SDL_FillRect(this->_background, NULL, 0x000000FF);
}

DisplaySdl::~DisplaySdl(void)
{
    this->_clean();
}

void DisplaySdl::_error(void)
{
    this->_clean();
    throw(DisplaySdl::SdlConstructorException(SDL_GetError()));
}

void DisplaySdl::_clean(void)
{
    if (this->_win)
	{
        SDL_DestroyWindow(this->_win);
        this->_win = NULL;
    }
    if (this->_rendererSurface)
	{
        SDL_FreeSurface(this->_rendererSurface);
        this->_rendererSurface = NULL;
    }
    if (this->_rendererTexture)
	{
        SDL_DestroyTexture(this->_rendererTexture);
        this->_rendererTexture = NULL;
    }
    if (this->_renderer)
	{
        SDL_DestroyRenderer(this->_renderer);
        this->_renderer = NULL;
    }
	if (this->_tileset)
	{
		SDL_DestroyRenderer(this->_renderer);
		this->_renderer = NULL;
	}
	if (this->_background)
	{
		SDL_DestroyRenderer(this->_renderer);
		this->_renderer = NULL;
	}
	IMG_Quit();
    SDL_Quit();
}

void DisplaySdl::render(float currentDelayFrame, float maxDelayFrame) {
    if (this->_rendererTexture)
        SDL_DestroyTexture(this->_rendererTexture);
    this->_rendererTexture = SDL_CreateTextureFromSurface(this->_renderer, this->_rendererSurface);
    SDL_RenderCopy(this->_renderer, this->_rendererTexture, NULL, NULL);
    SDL_RenderPresent(this->_renderer);
    SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255);
    SDL_RenderClear(this->_renderer);
    SDL_BlitSurface(this->_background, NULL, this->_rendererSurface, NULL);
}

SDL_Rect	DisplaySdl::_getRectTile(int width, int height)
{
	SDL_Rect rect = { this->_tileSize * width, this->_tileSize * height, this->_tileSize, this->_tileSize };
	return (rect);
}
SDL_Rect	DisplaySdl::_getRectTilePixel(int width, int height)
{
	SDL_Rect rect = { width, height, this->_tileSize, this->_tileSize };
	return (rect);
}

/*
**####################ID_TILE
*/

void			DisplaySdl::_drawTileGrid(int indexTile,
										int indexWidthGrid, int indexHeightGrid)
{
	SDL_Rect rectToDraw = this->_getRectTile(indexWidthGrid, indexHeightGrid);
	SDL_Rect rectTilset = this->_getRectTile(indexTile % this->_tilesetWidth, indexTile / this->_tilesetWidth);
	SDL_BlitSurface(this->_tileset, &rectTilset, this->_rendererSurface, &rectToDraw);
}
void			DisplaySdl::_drawTileGrid(SDL_Surface *surface,
										int indexTile,
										int indexWidthGrid, int indexHeightGrid)
{
	SDL_Rect rectToDraw = this->_getRectTile(indexWidthGrid, indexHeightGrid);
	SDL_Rect rectTilset = this->_getRectTile(indexTile % this->_tilesetWidth, indexTile / this->_tilesetWidth);
	SDL_BlitSurface(this->_tileset, &rectTilset, surface, &rectToDraw);
}

/*
**####################DRAW_GRID
*/

void			DisplaySdl::drawGrid(Grid< eSprite > const &grid)
{
	for (int y = 0; y < this->_winTileSize.getY(); ++y)
		for (int x = 0; x < this->_winTileSize.getX(); ++x)
			if (grid(x, y) != eSprite::NONE)
				this->_drawTileGrid(Display2D::getSpriteSnake(grid(x, y)), x, y);
}
void			DisplaySdl::_drawGrid(SDL_Surface *surface, Grid< eSprite > const &grid)
{
	for (int y = 0; y < this->_winTileSize.getY(); ++y)
		for (int x = 0; x < this->_winTileSize.getX(); ++x)
			if (grid(x, y) != eSprite::NONE)
				this->_drawTileGrid(surface, Display2D::getSpriteSnake(grid(x, y)), x, y);
}

void		DisplaySdl::setBackground(Grid< eSprite > const &grid)
{
	this->_drawGrid(this->_background, grid);
}

void			DisplaySdl::update(float deltaTime)
{

    while (SDL_PollEvent(&this->_ev))
	{
        if (this->_ev.window.event == SDL_WINDOWEVENT_CLOSE)
            this->_exit = true;
		if (this->_ev.type == SDL_KEYDOWN)
		{
			if (this->_ev.key.keysym.scancode == SDL_SCANCODE_A)
				this->_direction = kWest;
			else if (this->_ev.key.keysym.scancode == SDL_SCANCODE_D)
				this->_direction = kEast;
			else if (this->_ev.key.keysym.scancode == SDL_SCANCODE_W)
				this->_direction = kNorth;
			else if (this->_ev.key.keysym.scancode == SDL_SCANCODE_S)
				this->_direction = kSouth;
		}
    }
}

eDirection  	DisplaySdl::getDirection(void) const
{
	return (this->_direction);
}


bool			DisplaySdl::exit(void) const
{
    return (this->_exit);
}

SDL_Surface		*DisplaySdl::_SdlSurface(int widht, int height)
{
    return (SDL_CreateRGBSurface(0, widht, height,
            32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff));
}

void DisplaySdl::registerCallbackAction(std::function<void(eAction)> function) {
	_callback = function;
}

DisplaySdl::DisplaySdl(void) {

}

DisplaySdl::SdlConstructorException::~SdlConstructorException(void) throw(){}
DisplaySdl::SdlConstructorException::SdlConstructorException(void) throw() :
	_error("Error on Sdl constructor") {}
DisplaySdl::SdlConstructorException::SdlConstructorException(std::string s) throw() :
	_error(s) { }
DisplaySdl::SdlConstructorException::SdlConstructorException(DisplaySdl::SdlConstructorException const &src) throw() :
	_error(src._error)
	{ this->_error = src._error; }
const char	*DisplaySdl::SdlConstructorException::what() const throw()
	{ return (this->_error.c_str()); }
