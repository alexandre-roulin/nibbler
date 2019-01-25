
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
			switch (_ev.key.keysym.scancode) {

				case SDL_SCANCODE_UNKNOWN:break;
				case SDL_SCANCODE_A:
					this->_direction = kWest;
					break;
				case SDL_SCANCODE_B:break;
				case SDL_SCANCODE_C:break;
				case SDL_SCANCODE_D:
					this->_direction = kEast;
					break;
				case SDL_SCANCODE_E:break;
				case SDL_SCANCODE_F:break;
				case SDL_SCANCODE_G:break;
				case SDL_SCANCODE_H:break;
				case SDL_SCANCODE_I:break;
				case SDL_SCANCODE_J:break;
				case SDL_SCANCODE_K:break;
				case SDL_SCANCODE_L:break;
				case SDL_SCANCODE_M:break;
				case SDL_SCANCODE_N:break;
				case SDL_SCANCODE_O:
					_callback(eAction::kSwitchDisplayLibrary);
					break;
				case SDL_SCANCODE_P:
					_callback(eAction::kPause);
					break;
				case SDL_SCANCODE_Q:break;
				case SDL_SCANCODE_R:break;
				case SDL_SCANCODE_S:
					this->_direction = kSouth;
					break;
				case SDL_SCANCODE_T:break;
				case SDL_SCANCODE_U:break;
				case SDL_SCANCODE_V:break;
				case SDL_SCANCODE_W:
					this->_direction = kNorth;
					break;

				case SDL_SCANCODE_X:break;
				case SDL_SCANCODE_Y:break;
				case SDL_SCANCODE_Z:break;
				case SDL_SCANCODE_1:break;
				case SDL_SCANCODE_2:break;
				case SDL_SCANCODE_3:break;
				case SDL_SCANCODE_4:break;
				case SDL_SCANCODE_5:break;
				case SDL_SCANCODE_6:break;
				case SDL_SCANCODE_7:break;
				case SDL_SCANCODE_8:break;
				case SDL_SCANCODE_9:break;
				case SDL_SCANCODE_0:break;
				case SDL_SCANCODE_RETURN:break;
				case SDL_SCANCODE_ESCAPE:break;
				case SDL_SCANCODE_BACKSPACE:break;
				case SDL_SCANCODE_TAB:break;
				case SDL_SCANCODE_SPACE:break;
				case SDL_SCANCODE_MINUS:break;
				case SDL_SCANCODE_EQUALS:break;
				case SDL_SCANCODE_LEFTBRACKET:
					break;
				case SDL_SCANCODE_RIGHTBRACKET:
					break;
				case SDL_SCANCODE_BACKSLASH:break;
				case SDL_SCANCODE_NONUSHASH:break;
				case SDL_SCANCODE_SEMICOLON:break;
				case SDL_SCANCODE_APOSTROPHE:break;
				case SDL_SCANCODE_GRAVE:break;
				case SDL_SCANCODE_COMMA:break;
				case SDL_SCANCODE_PERIOD:break;
				case SDL_SCANCODE_SLASH:break;
				case SDL_SCANCODE_CAPSLOCK:break;
				case SDL_SCANCODE_F1:break;
				case SDL_SCANCODE_F2:break;
				case SDL_SCANCODE_F3:break;
				case SDL_SCANCODE_F4:break;
				case SDL_SCANCODE_F5:break;
				case SDL_SCANCODE_F6:break;
				case SDL_SCANCODE_F7:break;
				case SDL_SCANCODE_F8:break;
				case SDL_SCANCODE_F9:break;
				case SDL_SCANCODE_F10:break;
				case SDL_SCANCODE_F11:break;
				case SDL_SCANCODE_F12:break;
				case SDL_SCANCODE_PRINTSCREEN:break;
				case SDL_SCANCODE_SCROLLLOCK:break;
				case SDL_SCANCODE_PAUSE:break;
				case SDL_SCANCODE_INSERT:break;
				case SDL_SCANCODE_HOME:break;
				case SDL_SCANCODE_PAGEUP:break;
				case SDL_SCANCODE_DELETE:break;
				case SDL_SCANCODE_END:break;
				case SDL_SCANCODE_PAGEDOWN:break;
				case SDL_SCANCODE_RIGHT:
					this->_direction = kEast;
					break;
				case SDL_SCANCODE_LEFT:
					this->_direction = kWest;
					break;
				case SDL_SCANCODE_DOWN:
					this->_direction = kSouth;
					break;
				case SDL_SCANCODE_UP:
					this->_direction = kNorth;
					break;
				case SDL_SCANCODE_NUMLOCKCLEAR:break;
				case SDL_SCANCODE_KP_DIVIDE:break;
				case SDL_SCANCODE_KP_MULTIPLY:break;
				case SDL_SCANCODE_KP_MINUS:break;
				case SDL_SCANCODE_KP_PLUS:break;
				case SDL_SCANCODE_KP_ENTER:break;
				case SDL_SCANCODE_KP_1:break;
				case SDL_SCANCODE_KP_2:break;
				case SDL_SCANCODE_KP_3:break;
				case SDL_SCANCODE_KP_4:break;
				case SDL_SCANCODE_KP_5:break;
				case SDL_SCANCODE_KP_6:break;
				case SDL_SCANCODE_KP_7:break;
				case SDL_SCANCODE_KP_8:break;
				case SDL_SCANCODE_KP_9:break;
				case SDL_SCANCODE_KP_0:break;
				case SDL_SCANCODE_KP_PERIOD:break;
				case SDL_SCANCODE_NONUSBACKSLASH:break;
				case SDL_SCANCODE_APPLICATION:break;
				case SDL_SCANCODE_POWER:break;
				case SDL_SCANCODE_KP_EQUALS:break;
				case SDL_SCANCODE_F13:break;
				case SDL_SCANCODE_F14:break;
				case SDL_SCANCODE_F15:break;
				case SDL_SCANCODE_F16:break;
				case SDL_SCANCODE_F17:break;
				case SDL_SCANCODE_F18:break;
				case SDL_SCANCODE_F19:break;
				case SDL_SCANCODE_F20:break;
				case SDL_SCANCODE_F21:break;
				case SDL_SCANCODE_F22:break;
				case SDL_SCANCODE_F23:break;
				case SDL_SCANCODE_F24:break;
				case SDL_SCANCODE_EXECUTE:break;
				case SDL_SCANCODE_HELP:break;
				case SDL_SCANCODE_MENU:break;
				case SDL_SCANCODE_SELECT:break;
				case SDL_SCANCODE_STOP:break;
				case SDL_SCANCODE_AGAIN:break;
				case SDL_SCANCODE_UNDO:break;
				case SDL_SCANCODE_CUT:break;
				case SDL_SCANCODE_COPY:break;
				case SDL_SCANCODE_PASTE:break;
				case SDL_SCANCODE_FIND:break;
				case SDL_SCANCODE_MUTE:break;
				case SDL_SCANCODE_VOLUMEUP:break;
				case SDL_SCANCODE_VOLUMEDOWN:break;
				case SDL_SCANCODE_KP_COMMA:break;
				case SDL_SCANCODE_KP_EQUALSAS400:break;
				case SDL_SCANCODE_INTERNATIONAL1:break;
				case SDL_SCANCODE_INTERNATIONAL2:break;
				case SDL_SCANCODE_INTERNATIONAL3:break;
				case SDL_SCANCODE_INTERNATIONAL4:break;
				case SDL_SCANCODE_INTERNATIONAL5:break;
				case SDL_SCANCODE_INTERNATIONAL6:break;
				case SDL_SCANCODE_INTERNATIONAL7:break;
				case SDL_SCANCODE_INTERNATIONAL8:break;
				case SDL_SCANCODE_INTERNATIONAL9:break;
				case SDL_SCANCODE_LANG1:break;
				case SDL_SCANCODE_LANG2:break;
				case SDL_SCANCODE_LANG3:break;
				case SDL_SCANCODE_LANG4:break;
				case SDL_SCANCODE_LANG5:break;
				case SDL_SCANCODE_LANG6:break;
				case SDL_SCANCODE_LANG7:break;
				case SDL_SCANCODE_LANG8:break;
				case SDL_SCANCODE_LANG9:break;
				case SDL_SCANCODE_ALTERASE:break;
				case SDL_SCANCODE_SYSREQ:break;
				case SDL_SCANCODE_CANCEL:break;
				case SDL_SCANCODE_CLEAR:break;
				case SDL_SCANCODE_PRIOR:break;
				case SDL_SCANCODE_RETURN2:break;
				case SDL_SCANCODE_SEPARATOR:break;
				case SDL_SCANCODE_OUT:break;
				case SDL_SCANCODE_OPER:break;
				case SDL_SCANCODE_CLEARAGAIN:break;
				case SDL_SCANCODE_CRSEL:break;
				case SDL_SCANCODE_EXSEL:break;
				case SDL_SCANCODE_KP_00:break;
				case SDL_SCANCODE_KP_000:break;
				case SDL_SCANCODE_THOUSANDSSEPARATOR:break;
				case SDL_SCANCODE_DECIMALSEPARATOR:break;
				case SDL_SCANCODE_CURRENCYUNIT:break;
				case SDL_SCANCODE_CURRENCYSUBUNIT:break;
				case SDL_SCANCODE_KP_LEFTPAREN:break;
				case SDL_SCANCODE_KP_RIGHTPAREN:break;
				case SDL_SCANCODE_KP_LEFTBRACE:break;
				case SDL_SCANCODE_KP_RIGHTBRACE:break;
				case SDL_SCANCODE_KP_TAB:break;
				case SDL_SCANCODE_KP_BACKSPACE:break;
				case SDL_SCANCODE_KP_A:break;
				case SDL_SCANCODE_KP_B:break;
				case SDL_SCANCODE_KP_C:break;
				case SDL_SCANCODE_KP_D:break;
				case SDL_SCANCODE_KP_E:break;
				case SDL_SCANCODE_KP_F:break;
				case SDL_SCANCODE_KP_XOR:break;
				case SDL_SCANCODE_KP_POWER:break;
				case SDL_SCANCODE_KP_PERCENT:break;
				case SDL_SCANCODE_KP_LESS:break;
				case SDL_SCANCODE_KP_GREATER:break;
				case SDL_SCANCODE_KP_AMPERSAND:break;
				case SDL_SCANCODE_KP_DBLAMPERSAND:break;
				case SDL_SCANCODE_KP_VERTICALBAR:break;
				case SDL_SCANCODE_KP_DBLVERTICALBAR:break;
				case SDL_SCANCODE_KP_COLON:break;
				case SDL_SCANCODE_KP_HASH:break;
				case SDL_SCANCODE_KP_SPACE:break;
				case SDL_SCANCODE_KP_AT:break;
				case SDL_SCANCODE_KP_EXCLAM:break;
				case SDL_SCANCODE_KP_MEMSTORE:break;
				case SDL_SCANCODE_KP_MEMRECALL:break;
				case SDL_SCANCODE_KP_MEMCLEAR:break;
				case SDL_SCANCODE_KP_MEMADD:break;
				case SDL_SCANCODE_KP_MEMSUBTRACT:break;
				case SDL_SCANCODE_KP_MEMMULTIPLY:break;
				case SDL_SCANCODE_KP_MEMDIVIDE:break;
				case SDL_SCANCODE_KP_PLUSMINUS:break;
				case SDL_SCANCODE_KP_CLEAR:break;
				case SDL_SCANCODE_KP_CLEARENTRY:break;
				case SDL_SCANCODE_KP_BINARY:break;
				case SDL_SCANCODE_KP_OCTAL:break;
				case SDL_SCANCODE_KP_DECIMAL:break;
				case SDL_SCANCODE_KP_HEXADECIMAL:break;
				case SDL_SCANCODE_LCTRL:break;
				case SDL_SCANCODE_LSHIFT:break;
				case SDL_SCANCODE_LALT:break;
				case SDL_SCANCODE_LGUI:break;
				case SDL_SCANCODE_RCTRL:break;
				case SDL_SCANCODE_RSHIFT:break;
				case SDL_SCANCODE_RALT:break;
				case SDL_SCANCODE_RGUI:break;
				case SDL_SCANCODE_MODE:break;
				case SDL_SCANCODE_AUDIONEXT:break;
				case SDL_SCANCODE_AUDIOPREV:break;
				case SDL_SCANCODE_AUDIOSTOP:break;
				case SDL_SCANCODE_AUDIOPLAY:break;
				case SDL_SCANCODE_AUDIOMUTE:break;
				case SDL_SCANCODE_MEDIASELECT:break;
				case SDL_SCANCODE_WWW:break;
				case SDL_SCANCODE_MAIL:break;
				case SDL_SCANCODE_CALCULATOR:break;
				case SDL_SCANCODE_COMPUTER:break;
				case SDL_SCANCODE_AC_SEARCH:break;
				case SDL_SCANCODE_AC_HOME:break;
				case SDL_SCANCODE_AC_BACK:break;
				case SDL_SCANCODE_AC_FORWARD:break;
				case SDL_SCANCODE_AC_STOP:break;
				case SDL_SCANCODE_AC_REFRESH:break;
				case SDL_SCANCODE_AC_BOOKMARKS:break;
				case SDL_SCANCODE_BRIGHTNESSDOWN:break;
				case SDL_SCANCODE_BRIGHTNESSUP:break;
				case SDL_SCANCODE_DISPLAYSWITCH:break;
				case SDL_SCANCODE_KBDILLUMTOGGLE:break;
				case SDL_SCANCODE_KBDILLUMDOWN:break;
				case SDL_SCANCODE_KBDILLUMUP:break;
				case SDL_SCANCODE_EJECT:break;
				case SDL_SCANCODE_SLEEP:break;
				case SDL_SCANCODE_APP1:break;
				case SDL_SCANCODE_APP2:break;
				case SDL_SCANCODE_AUDIOREWIND:break;
				case SDL_SCANCODE_AUDIOFASTFORWARD:break;
				case SDL_NUM_SCANCODES:break;
			}
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
