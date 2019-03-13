
#include <boost/filesystem.hpp>
#include "nibbler.hpp"
#include "DisplaySdl.hpp"
#include "Display2D.hpp"

IDisplay *newInstance(int width,
					 int height,
					 char const *windowName) {
	return (new DisplaySdl(width, height, windowName));
}

void deleteInstance(IDisplay *display) {
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
				case SDL_SCANCODE_ESCAPE:
					exit_ = true;
					break;
				case SDL_SCANCODE_A:
					direction_ = kWest;
					break;
				case SDL_SCANCODE_D:
					direction_ = kEast;
					break;
				case SDL_SCANCODE_O:
					callback_(eAction::kSwitchDisplayLibrary);
					break;
				case SDL_SCANCODE_P:
					callback_(eAction::kPause);
					break;
				case SDL_SCANCODE_S:
					direction_ = kSouth;
					break;
				case SDL_SCANCODE_W:
					direction_ = kNorth;
					break;
				case SDL_SCANCODE_LEFT:
					direction_ = kWest;
					break;
				case SDL_SCANCODE_RIGHT:
					direction_ = kEast;
					break;
				case SDL_SCANCODE_DOWN:
					direction_ = kSouth;
					break;
				case SDL_SCANCODE_UP:
					direction_ = kNorth;
					break;
				default:
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
