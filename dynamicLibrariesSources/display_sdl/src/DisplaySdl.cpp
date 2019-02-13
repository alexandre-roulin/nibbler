
#include <boost/filesystem.hpp>
#include "nibbler.hpp"
#include "DisplaySdl.hpp"
#include "Display2D.hpp"

IDisplay *newDisplay(int width,
					 int height,
					 char const *windowName) {
	return (new DisplaySdl(width, height, windowName));
}

void deleteDisplay(IDisplay *display) {
	delete display;
}

DisplaySdl::DisplaySdl(int width,
					   int height,
					   char const *windowName) :
		exit_(false),
		direction_(kNorth),
		tileSize_(DISPLAY_DEFAULT_TILE_SIZE),
		winTileSize_(Vector2D<int>(width, height)),
		winPixelSize_(Vector2D<int>(width * tileSize_, height * tileSize_)),
		win_(NULL),
		rendererSurface_(NULL),
		rendererTexture_(NULL),
		renderer_(NULL) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0 || IMG_Init(IMG_INIT_PNG) < 0)
		error_();

	boost::filesystem::path pathRoot(NIBBLER_ROOT_PROJECT_PATH);

	if (!(tileset_ = IMG_Load((pathRoot / "ressources" / "snake_tileset.png").generic_string().c_str())))
		error_();
	tilesetWidth_ = (tileset_->w / tileSize_);
	if (!(win_ = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED,
								  SDL_WINDOWPOS_CENTERED, winPixelSize_.getX(), winPixelSize_.getY(),
								  SDL_WINDOW_SHOWN)))
		error_();

	if (!(renderer_ = SDL_CreateRenderer(win_, -1, SDL_RENDERER_ACCELERATED)))
		error_();
	if (!(rendererSurface_ = DisplaySdl::sdlSurface_(winPixelSize_.getX(), winPixelSize_.getY())))
		error_();
	if (!(background_ = DisplaySdl::sdlSurface_(winPixelSize_.getX(), winPixelSize_.getY())))
		error_();
	SDL_FillRect(background_, NULL, 0x000000FF);
}

DisplaySdl::~DisplaySdl(void) {
	clean_();
}

void DisplaySdl::error_(void) {
	clean_();
	throw (DisplaySdl::SdlConstructorException(SDL_GetError()));
}

void DisplaySdl::clean_(void) {
	if (win_) {
		SDL_DestroyWindow(win_);
		win_ = NULL;
	}
	if (rendererSurface_) {
		SDL_FreeSurface(rendererSurface_);
		rendererSurface_ = NULL;
	}
	if (rendererTexture_) {
		SDL_DestroyTexture(rendererTexture_);
		rendererTexture_ = NULL;
	}
	if (renderer_) {
		SDL_DestroyRenderer(renderer_);
		renderer_ = NULL;
	}
	if (tileset_) {
		SDL_DestroyRenderer(renderer_);
		renderer_ = NULL;
	}
	if (background_) {
		SDL_DestroyRenderer(renderer_);
		renderer_ = NULL;
	}
	IMG_Quit();
	SDL_Quit();
}

void DisplaySdl::render() {
	if (rendererTexture_)
		SDL_DestroyTexture(rendererTexture_);
	rendererTexture_ = SDL_CreateTextureFromSurface(renderer_, rendererSurface_);
	SDL_RenderCopy(renderer_, rendererTexture_, NULL, NULL);
	SDL_RenderPresent(renderer_);
	SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
	SDL_RenderClear(renderer_);
	SDL_BlitSurface(background_, NULL, rendererSurface_, NULL);
}

SDL_Rect DisplaySdl::getRectTile_(int width, int height) {
	SDL_Rect rect = {tileSize_ * width, tileSize_ * height, tileSize_, tileSize_};
	return (rect);
}

/*
**####################ID_TILE
*/

void DisplaySdl::drawTileGrid_(int indexTile,
							   int indexWidthGrid, int indexHeightGrid) {
	SDL_Rect rectToDraw = getRectTile_(indexWidthGrid, indexHeightGrid);
	SDL_Rect rectTilset = getRectTile_(indexTile % tilesetWidth_, indexTile / tilesetWidth_);
	SDL_BlitSurface(tileset_, &rectTilset, rendererSurface_, &rectToDraw);
}

void DisplaySdl::drawTileGrid_(SDL_Surface *surface,
							   int indexTile,
							   int indexWidthGrid, int indexHeightGrid) {
	SDL_Rect rectToDraw = getRectTile_(indexWidthGrid, indexHeightGrid);
	SDL_Rect rectTilset = getRectTile_(indexTile % tilesetWidth_, indexTile / tilesetWidth_);
	SDL_BlitSurface(tileset_, &rectTilset, surface, &rectToDraw);
}

/*
**####################DRAW_GRID
*/

void DisplaySdl::drawGrid(MutantGrid<eSprite> const &grid) {
	for (int y = 0; y < winTileSize_.getY(); ++y)
		for (int x = 0; x < winTileSize_.getX(); ++x)
			if (grid(x, y) != eSprite::kNone)
				drawTileGrid_(Display2D::getSpriteSnake(grid(x, y)), x, y);
}

void DisplaySdl::drawGrid_(SDL_Surface *surface, MutantGrid<eSprite> const &grid) {
	for (int y = 0; y < winTileSize_.getY(); ++y)
		for (int x = 0; x < winTileSize_.getX(); ++x)
			if (grid(x, y) != eSprite::kNone)
				drawTileGrid_(surface, Display2D::getSpriteSnake(grid(x, y)), x, y);
}

void DisplaySdl::setBackground(MutantGrid<eSprite> const &grid) {
	drawGrid_(background_, grid);
}

void DisplaySdl::update() {

	while (SDL_PollEvent(&ev_)) {
		if (ev_.window.event == SDL_WINDOWEVENT_CLOSE)
			exit_ = true;
		if (ev_.type == SDL_KEYDOWN) {
			switch (ev_.key.keysym.scancode) {

				case SDL_SCANCODE_UNKNOWN:
					break;
				case SDL_SCANCODE_A:
					direction_ = kWest;
					break;
				case SDL_SCANCODE_B:
					break;
				case SDL_SCANCODE_C:
					break;
				case SDL_SCANCODE_D:
					direction_ = kEast;
					break;
				case SDL_SCANCODE_E:
					break;
				case SDL_SCANCODE_F:
					break;
				case SDL_SCANCODE_G:
					break;
				case SDL_SCANCODE_H:
					break;
				case SDL_SCANCODE_I:
					break;
				case SDL_SCANCODE_J:
					break;
				case SDL_SCANCODE_K:
					break;
				case SDL_SCANCODE_L:
					break;
				case SDL_SCANCODE_M:
					break;
				case SDL_SCANCODE_N:
					break;
				case SDL_SCANCODE_O:
					callback_(eAction::kSwitchDisplayLibrary);
					break;
				case SDL_SCANCODE_P:
					callback_(eAction::kPause);
					break;
				case SDL_SCANCODE_Q:
					break;
				case SDL_SCANCODE_R:
					break;
				case SDL_SCANCODE_S:
					direction_ = kSouth;
					break;
				case SDL_SCANCODE_T:
					break;
				case SDL_SCANCODE_U:
					break;
				case SDL_SCANCODE_V:
					break;
				case SDL_SCANCODE_W:
					direction_ = kNorth;
					break;

				case SDL_SCANCODE_X:
					break;
				case SDL_SCANCODE_Y:
					break;
				case SDL_SCANCODE_Z:
					break;
				case SDL_SCANCODE_1:
					break;
				case SDL_SCANCODE_2:
					break;
				case SDL_SCANCODE_3:
					break;
				case SDL_SCANCODE_4:
					break;
				case SDL_SCANCODE_5:
					break;
				case SDL_SCANCODE_6:
					break;
				case SDL_SCANCODE_7:
					break;
				case SDL_SCANCODE_8:
					break;
				case SDL_SCANCODE_9:
					break;
				case SDL_SCANCODE_0:
					break;
				case SDL_SCANCODE_RETURN:
					break;
				case SDL_SCANCODE_ESCAPE:
					break;
				case SDL_SCANCODE_BACKSPACE:
					break;
				case SDL_SCANCODE_TAB:
					break;
				case SDL_SCANCODE_SPACE:
					break;
				case SDL_SCANCODE_MINUS:
					break;
				case SDL_SCANCODE_EQUALS:
					break;
				case SDL_SCANCODE_LEFTBRACKET:
					break;
				case SDL_SCANCODE_RIGHTBRACKET:
					break;
				case SDL_SCANCODE_BACKSLASH:
					break;
				case SDL_SCANCODE_NONUSHASH:
					break;
				case SDL_SCANCODE_SEMICOLON:
					break;
				case SDL_SCANCODE_APOSTROPHE:
					break;
				case SDL_SCANCODE_GRAVE:
					break;
				case SDL_SCANCODE_COMMA:
					break;
				case SDL_SCANCODE_PERIOD:
					break;
				case SDL_SCANCODE_SLASH:
					break;
				case SDL_SCANCODE_CAPSLOCK:
					break;
				case SDL_SCANCODE_F1:
					break;
				case SDL_SCANCODE_F2:
					break;
				case SDL_SCANCODE_F3:
					break;
				case SDL_SCANCODE_F4:
					break;
				case SDL_SCANCODE_F5:
					break;
				case SDL_SCANCODE_F6:
					break;
				case SDL_SCANCODE_F7:
					break;
				case SDL_SCANCODE_F8:
					break;
				case SDL_SCANCODE_F9:
					break;
				case SDL_SCANCODE_F10:
					break;
				case SDL_SCANCODE_F11:
					break;
				case SDL_SCANCODE_F12:
					break;
				case SDL_SCANCODE_PRINTSCREEN:
					break;
				case SDL_SCANCODE_SCROLLLOCK:
					break;
				case SDL_SCANCODE_PAUSE:
					break;
				case SDL_SCANCODE_INSERT:
					break;
				case SDL_SCANCODE_HOME:
					break;
				case SDL_SCANCODE_PAGEUP:
					break;
				case SDL_SCANCODE_DELETE:
					break;
				case SDL_SCANCODE_END:
					break;
				case SDL_SCANCODE_PAGEDOWN:
					break;
				case SDL_SCANCODE_RIGHT:
					direction_ = kEast;
					break;
				case SDL_SCANCODE_LEFT:
					direction_ = kWest;
					break;
				case SDL_SCANCODE_DOWN:
					direction_ = kSouth;
					break;
				case SDL_SCANCODE_UP:
					direction_ = kNorth;
					break;
				case SDL_SCANCODE_NUMLOCKCLEAR:
					break;
				case SDL_SCANCODE_KP_DIVIDE:
					break;
				case SDL_SCANCODE_KP_MULTIPLY:
					break;
				case SDL_SCANCODE_KP_MINUS:
					break;
				case SDL_SCANCODE_KP_PLUS:
					break;
				case SDL_SCANCODE_KP_ENTER:
					break;
				case SDL_SCANCODE_KP_1:
					break;
				case SDL_SCANCODE_KP_2:
					break;
				case SDL_SCANCODE_KP_3:
					break;
				case SDL_SCANCODE_KP_4:
					break;
				case SDL_SCANCODE_KP_5:
					break;
				case SDL_SCANCODE_KP_6:
					break;
				case SDL_SCANCODE_KP_7:
					break;
				case SDL_SCANCODE_KP_8:
					break;
				case SDL_SCANCODE_KP_9:
					break;
				case SDL_SCANCODE_KP_0:
					break;
				case SDL_SCANCODE_KP_PERIOD:
					break;
				case SDL_SCANCODE_NONUSBACKSLASH:
					break;
				case SDL_SCANCODE_APPLICATION:
					break;
				case SDL_SCANCODE_POWER:
					break;
				case SDL_SCANCODE_KP_EQUALS:
					break;
				case SDL_SCANCODE_F13:
					break;
				case SDL_SCANCODE_F14:
					break;
				case SDL_SCANCODE_F15:
					break;
				case SDL_SCANCODE_F16:
					break;
				case SDL_SCANCODE_F17:
					break;
				case SDL_SCANCODE_F18:
					break;
				case SDL_SCANCODE_F19:
					break;
				case SDL_SCANCODE_F20:
					break;
				case SDL_SCANCODE_F21:
					break;
				case SDL_SCANCODE_F22:
					break;
				case SDL_SCANCODE_F23:
					break;
				case SDL_SCANCODE_F24:
					break;
				case SDL_SCANCODE_EXECUTE:
					break;
				case SDL_SCANCODE_HELP:
					break;
				case SDL_SCANCODE_MENU:
					break;
				case SDL_SCANCODE_SELECT:
					break;
				case SDL_SCANCODE_STOP:
					break;
				case SDL_SCANCODE_AGAIN:
					break;
				case SDL_SCANCODE_UNDO:
					break;
				case SDL_SCANCODE_CUT:
					break;
				case SDL_SCANCODE_COPY:
					break;
				case SDL_SCANCODE_PASTE:
					break;
				case SDL_SCANCODE_FIND:
					break;
				case SDL_SCANCODE_MUTE:
					break;
				case SDL_SCANCODE_VOLUMEUP:
					break;
				case SDL_SCANCODE_VOLUMEDOWN:
					break;
				case SDL_SCANCODE_KP_COMMA:
					break;
				case SDL_SCANCODE_KP_EQUALSAS400:
					break;
				case SDL_SCANCODE_INTERNATIONAL1:
					break;
				case SDL_SCANCODE_INTERNATIONAL2:
					break;
				case SDL_SCANCODE_INTERNATIONAL3:
					break;
				case SDL_SCANCODE_INTERNATIONAL4:
					break;
				case SDL_SCANCODE_INTERNATIONAL5:
					break;
				case SDL_SCANCODE_INTERNATIONAL6:
					break;
				case SDL_SCANCODE_INTERNATIONAL7:
					break;
				case SDL_SCANCODE_INTERNATIONAL8:
					break;
				case SDL_SCANCODE_INTERNATIONAL9:
					break;
				case SDL_SCANCODE_LANG1:
					break;
				case SDL_SCANCODE_LANG2:
					break;
				case SDL_SCANCODE_LANG3:
					break;
				case SDL_SCANCODE_LANG4:
					break;
				case SDL_SCANCODE_LANG5:
					break;
				case SDL_SCANCODE_LANG6:
					break;
				case SDL_SCANCODE_LANG7:
					break;
				case SDL_SCANCODE_LANG8:
					break;
				case SDL_SCANCODE_LANG9:
					break;
				case SDL_SCANCODE_ALTERASE:
					break;
				case SDL_SCANCODE_SYSREQ:
					break;
				case SDL_SCANCODE_CANCEL:
					break;
				case SDL_SCANCODE_CLEAR:
					break;
				case SDL_SCANCODE_PRIOR:
					break;
				case SDL_SCANCODE_RETURN2:
					break;
				case SDL_SCANCODE_SEPARATOR:
					break;
				case SDL_SCANCODE_OUT:
					break;
				case SDL_SCANCODE_OPER:
					break;
				case SDL_SCANCODE_CLEARAGAIN:
					break;
				case SDL_SCANCODE_CRSEL:
					break;
				case SDL_SCANCODE_EXSEL:
					break;
				case SDL_SCANCODE_KP_00:
					break;
				case SDL_SCANCODE_KP_000:
					break;
				case SDL_SCANCODE_THOUSANDSSEPARATOR:
					break;
				case SDL_SCANCODE_DECIMALSEPARATOR:
					break;
				case SDL_SCANCODE_CURRENCYUNIT:
					break;
				case SDL_SCANCODE_CURRENCYSUBUNIT:
					break;
				case SDL_SCANCODE_KP_LEFTPAREN:
					break;
				case SDL_SCANCODE_KP_RIGHTPAREN:
					break;
				case SDL_SCANCODE_KP_LEFTBRACE:
					break;
				case SDL_SCANCODE_KP_RIGHTBRACE:
					break;
				case SDL_SCANCODE_KP_TAB:
					break;
				case SDL_SCANCODE_KP_BACKSPACE:
					break;
				case SDL_SCANCODE_KP_A:
					break;
				case SDL_SCANCODE_KP_B:
					break;
				case SDL_SCANCODE_KP_C:
					break;
				case SDL_SCANCODE_KP_D:
					break;
				case SDL_SCANCODE_KP_E:
					break;
				case SDL_SCANCODE_KP_F:
					break;
				case SDL_SCANCODE_KP_XOR:
					break;
				case SDL_SCANCODE_KP_POWER:
					break;
				case SDL_SCANCODE_KP_PERCENT:
					break;
				case SDL_SCANCODE_KP_LESS:
					break;
				case SDL_SCANCODE_KP_GREATER:
					break;
				case SDL_SCANCODE_KP_AMPERSAND:
					break;
				case SDL_SCANCODE_KP_DBLAMPERSAND:
					break;
				case SDL_SCANCODE_KP_VERTICALBAR:
					break;
				case SDL_SCANCODE_KP_DBLVERTICALBAR:
					break;
				case SDL_SCANCODE_KP_COLON:
					break;
				case SDL_SCANCODE_KP_HASH:
					break;
				case SDL_SCANCODE_KP_SPACE:
					break;
				case SDL_SCANCODE_KP_AT:
					break;
				case SDL_SCANCODE_KP_EXCLAM:
					break;
				case SDL_SCANCODE_KP_MEMSTORE:
					break;
				case SDL_SCANCODE_KP_MEMRECALL:
					break;
				case SDL_SCANCODE_KP_MEMCLEAR:
					break;
				case SDL_SCANCODE_KP_MEMADD:
					break;
				case SDL_SCANCODE_KP_MEMSUBTRACT:
					break;
				case SDL_SCANCODE_KP_MEMMULTIPLY:
					break;
				case SDL_SCANCODE_KP_MEMDIVIDE:
					break;
				case SDL_SCANCODE_KP_PLUSMINUS:
					break;
				case SDL_SCANCODE_KP_CLEAR:
					break;
				case SDL_SCANCODE_KP_CLEARENTRY:
					break;
				case SDL_SCANCODE_KP_BINARY:
					break;
				case SDL_SCANCODE_KP_OCTAL:
					break;
				case SDL_SCANCODE_KP_DECIMAL:
					break;
				case SDL_SCANCODE_KP_HEXADECIMAL:
					break;
				case SDL_SCANCODE_LCTRL:
					break;
				case SDL_SCANCODE_LSHIFT:
					break;
				case SDL_SCANCODE_LALT:
					break;
				case SDL_SCANCODE_LGUI:
					break;
				case SDL_SCANCODE_RCTRL:
					break;
				case SDL_SCANCODE_RSHIFT:
					break;
				case SDL_SCANCODE_RALT:
					break;
				case SDL_SCANCODE_RGUI:
					break;
				case SDL_SCANCODE_MODE:
					break;
				case SDL_SCANCODE_AUDIONEXT:
					break;
				case SDL_SCANCODE_AUDIOPREV:
					break;
				case SDL_SCANCODE_AUDIOSTOP:
					break;
				case SDL_SCANCODE_AUDIOPLAY:
					break;
				case SDL_SCANCODE_AUDIOMUTE:
					break;
				case SDL_SCANCODE_MEDIASELECT:
					break;
				case SDL_SCANCODE_WWW:
					break;
				case SDL_SCANCODE_MAIL:
					break;
				case SDL_SCANCODE_CALCULATOR:
					break;
				case SDL_SCANCODE_COMPUTER:
					break;
				case SDL_SCANCODE_AC_SEARCH:
					break;
				case SDL_SCANCODE_AC_HOME:
					break;
				case SDL_SCANCODE_AC_BACK:
					break;
				case SDL_SCANCODE_AC_FORWARD:
					break;
				case SDL_SCANCODE_AC_STOP:
					break;
				case SDL_SCANCODE_AC_REFRESH:
					break;
				case SDL_SCANCODE_AC_BOOKMARKS:
					break;
				case SDL_SCANCODE_BRIGHTNESSDOWN:
					break;
				case SDL_SCANCODE_BRIGHTNESSUP:
					break;
				case SDL_SCANCODE_DISPLAYSWITCH:
					break;
				case SDL_SCANCODE_KBDILLUMTOGGLE:
					break;
				case SDL_SCANCODE_KBDILLUMDOWN:
					break;
				case SDL_SCANCODE_KBDILLUMUP:
					break;
				case SDL_SCANCODE_EJECT:
					break;
				case SDL_SCANCODE_SLEEP:
					break;
				case SDL_SCANCODE_APP1:
					break;
				case SDL_SCANCODE_APP2:
					break;
				case SDL_SCANCODE_AUDIOREWIND:
					break;
				case SDL_SCANCODE_AUDIOFASTFORWARD:
					break;
				case SDL_NUM_SCANCODES:
					break;
			}
		}
	}
}

eDirection DisplaySdl::getDirection(void) const {
	return (direction_);
}


bool DisplaySdl::exit(void) const {
	return (exit_);
}

SDL_Surface *DisplaySdl::sdlSurface_(int widht, int height) {
	return (SDL_CreateRGBSurface(0, widht, height,
								 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff));
}

void DisplaySdl::registerCallbackAction(std::function<void(eAction)> function) {
	callback_ = function;
}

DisplaySdl::SdlConstructorException::SdlConstructorException(void) noexcept :
		error_("Error on Sdl constructor") {}

DisplaySdl::SdlConstructorException::SdlConstructorException(std::string const &s) noexcept :
		error_(s) {}

const char *DisplaySdl::SdlConstructorException::what() const noexcept { return (error_.c_str()); }
