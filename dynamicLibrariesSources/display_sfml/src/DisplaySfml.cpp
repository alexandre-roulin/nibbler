#include "nibbler.hpp"
#include <boost/filesystem.hpp>
#include "DisplaySfml.hpp"
#include "Display2D.hpp"

IDisplay *newDisplay(int width,
					 int height,
					 char const *windowName) {
	return (new DisplaySfml(width, height, windowName));
}

void deleteDisplay(IDisplay *display) {
	delete display;
}

DisplaySfml::DisplaySfml(int width,
						 int height,
						 char const *windowName) :
		exit_(false),
		direction_(kNorth),
		tileSize_(DISPLAY_DEFAULT_TILE_SIZE),
		winTileSize_(Vector2D<int>(width, height)),
		winPixelSize_(Vector2D<int>(width * tileSize_, height * tileSize_)),
		win_(sf::VideoMode(winPixelSize_.getX(),
						   winPixelSize_.getY()),
			 windowName) {


	boost::filesystem::path pathRoot(NIBBLER_ROOT_PROJECT_PATH);
	if (!tileset_.loadFromFile((pathRoot / "ressources" / "snake_tileset.png").generic_string()))
		_error("Tileset cannot be loaded");

	win_.setFramerateLimit(60);
	if (!textureBackground_.create(winPixelSize_.getX(),
								   winPixelSize_.getY()))
		_error("Error during creation of Texture background");
	tilesetWidth_ = (tileset_.getSize().x / tileSize_);
	win_.clear();
	textureBackground_.clear();
	win_.display();
}

DisplaySfml::~DisplaySfml(void) {
	_clean();
}

void DisplaySfml::_error(std::string const &s = std::string("Error")) {
	_clean();
	throw (DisplaySfml::SfmlConstructorException(s));
}

void DisplaySfml::_clean(void) {
	win_.close();
}

void DisplaySfml::render(float, float) {
	win_.display();
	win_.draw(spriteBackground_);
}

sf::VertexArray
DisplaySfml::getQuadTilePixel_(int indexWidthTile, int indexHeightTile,
							   int indexWidthGrid, int indexHeightGrid) {
	sf::VertexArray quad(sf::Quads, 4);
	quad[0].position = sf::Vector2f(((indexWidthGrid)), ((indexHeightGrid)));
	quad[1].position = sf::Vector2f(tileSize_, 0.f) + quad[0].position;

	quad[2].position = sf::Vector2f(((indexWidthGrid + tileSize_)),
									((indexHeightGrid + tileSize_)));
	quad[3].position = quad[2].position - sf::Vector2f(tileSize_, 0.f);

	quad[0].texCoords = sf::Vector2f((tileSize_) * (indexWidthTile),
									 (tileSize_) * (indexHeightTile));
	quad[1].texCoords = sf::Vector2f(tileSize_, 0.f) + quad[0].texCoords;

	quad[2].texCoords = sf::Vector2f((tileSize_) * (indexWidthTile + 1),
									 (tileSize_) * (indexHeightTile + 1));
	quad[3].texCoords = quad[2].texCoords - sf::Vector2f(tileSize_, 0.f);
	return (quad);
}

/*
**####################ID_TILE
*/

void DisplaySfml::drawTileGrid_(int indexTile, int indexWidthGrid,
								int indexHeightGrid) {
	win_.draw(getQuadTilePixel_(indexTile % tilesetWidth_,
								indexTile / tilesetWidth_,
								indexWidthGrid * tileSize_,
								indexHeightGrid * tileSize_),
			  &tileset_);
}

void DisplaySfml::drawTileGrid_(sf::RenderTarget &target, int indexTile,
								int indexWidthGrid, int indexHeightGrid) {
	target.draw(getQuadTilePixel_(indexTile % tilesetWidth_,
								  indexTile / tilesetWidth_,
								  indexWidthGrid * tileSize_,
								  indexHeightGrid * tileSize_),
				&tileset_);
}

void DisplaySfml::drawGrid(MutantGrid<eSprite> const &grid) {
	for (int y = 0; y < winTileSize_.getY(); ++y)
		for (int x = 0; x < winTileSize_.getX(); ++x)
			if (grid(x, y) != eSprite::kNone)
				drawTileGrid_(Display2D::getSpriteSnake(grid(x, y)), x, y);
}

void DisplaySfml::drawGrid_(sf::RenderTarget &target, MutantGrid<eSprite> const &grid) {
	for (int y = 0; y < winTileSize_.getY(); ++y)
		for (int x = 0; x < winTileSize_.getX(); ++x)
			if (grid(x, y) != eSprite::kNone)
				drawTileGrid_(target, Display2D::getSpriteSnake(grid(x, y)), x, y);
}

void DisplaySfml::setBackground(MutantGrid<eSprite> const &grid) {
	drawGrid_(textureBackground_, grid);
	textureBackground_.display();
	spriteBackground_ = sf::Sprite(textureBackground_.getTexture());
}

void DisplaySfml::update(float) {
	while (win_.pollEvent(ev_)) {
		if (ev_.type == sf::Event::Closed)
			exit_ = true;
		if (ev_.type == sf::Event::KeyPressed) {
			switch (ev_.key.code) {
				case sf::Keyboard::Unknown:
					break;
				case sf::Keyboard::A:
					direction_ = kWest;
					break;
				case sf::Keyboard::B:
					break;
				case sf::Keyboard::C:
					break;
				case sf::Keyboard::D:
					direction_ = kEast;
					break;
				case sf::Keyboard::E:
					break;
				case sf::Keyboard::F:
					break;
				case sf::Keyboard::G:
					break;
				case sf::Keyboard::H:
					break;
				case sf::Keyboard::I:
					break;
				case sf::Keyboard::J:
					break;
				case sf::Keyboard::K:
					break;
				case sf::Keyboard::L:
					break;
				case sf::Keyboard::M:
					break;
				case sf::Keyboard::N:
					break;
				case sf::Keyboard::O:
					callback_(eAction::kSwitchDisplayLibrary);
					break;
				case sf::Keyboard::P:
					callback_(eAction::kPause);
					break;
				case sf::Keyboard::Q:
					break;
				case sf::Keyboard::R:
					break;
				case sf::Keyboard::S:
					direction_ = kSouth;
					break;
				case sf::Keyboard::T:
					break;
				case sf::Keyboard::U:
					break;
				case sf::Keyboard::V:
					break;
				case sf::Keyboard::W:
					direction_ = kNorth;

					break;
				case sf::Keyboard::X:
					break;
				case sf::Keyboard::Y:
					break;
				case sf::Keyboard::Z:
					break;
				case sf::Keyboard::Num0:
					break;
				case sf::Keyboard::Num1:
					break;
				case sf::Keyboard::Num2:
					break;
				case sf::Keyboard::Num3:
					break;
				case sf::Keyboard::Num4:
					break;
				case sf::Keyboard::Num5:
					break;
				case sf::Keyboard::Num6:
					break;
				case sf::Keyboard::Num7:
					break;
				case sf::Keyboard::Num8:
					break;
				case sf::Keyboard::Num9:
					break;
				case sf::Keyboard::Escape:
					break;
				case sf::Keyboard::LControl:
					break;
				case sf::Keyboard::LShift:
					break;
				case sf::Keyboard::LAlt:
					break;
				case sf::Keyboard::LSystem:
					break;
				case sf::Keyboard::RControl:
					break;
				case sf::Keyboard::RShift:
					break;
				case sf::Keyboard::RAlt:
					break;
				case sf::Keyboard::RSystem:
					break;
				case sf::Keyboard::Menu:
					break;
				case sf::Keyboard::LBracket:
					break;
				case sf::Keyboard::RBracket:
					break;
				case sf::Keyboard::SemiColon:
					break;
				case sf::Keyboard::Comma:
					break;
				case sf::Keyboard::Period:
					break;
				case sf::Keyboard::Quote:
					break;
				case sf::Keyboard::Slash:
					break;
				case sf::Keyboard::BackSlash:
					break;
				case sf::Keyboard::Tilde:
					break;
				case sf::Keyboard::Equal:
					break;
				case sf::Keyboard::Dash:
					break;
				case sf::Keyboard::Space:
					break;
				case sf::Keyboard::Return:
					break;
				case sf::Keyboard::BackSpace:
					break;
				case sf::Keyboard::Tab:
					break;
				case sf::Keyboard::PageUp:
					break;
				case sf::Keyboard::PageDown:
					break;
				case sf::Keyboard::End:
					break;
				case sf::Keyboard::Home:
					break;
				case sf::Keyboard::Insert:
					break;
				case sf::Keyboard::Delete:
					break;
				case sf::Keyboard::Add:
					break;
				case sf::Keyboard::Subtract:
					break;
				case sf::Keyboard::Multiply:
					break;
				case sf::Keyboard::Divide:
					break;
				case sf::Keyboard::Left:
					direction_ = kWest;
					break;
				case sf::Keyboard::Right:
					direction_ = kEast;
					break;
				case sf::Keyboard::Up:
					direction_ = kNorth;
					break;
				case sf::Keyboard::Down:
					direction_ = kSouth;
					break;
				case sf::Keyboard::Numpad0:
					break;
				case sf::Keyboard::Numpad1:
					break;
				case sf::Keyboard::Numpad2:
					break;
				case sf::Keyboard::Numpad3:
					break;
				case sf::Keyboard::Numpad4:
					break;
				case sf::Keyboard::Numpad5:
					break;
				case sf::Keyboard::Numpad6:
					break;
				case sf::Keyboard::Numpad7:
					break;
				case sf::Keyboard::Numpad8:
					break;
				case sf::Keyboard::Numpad9:
					break;
				case sf::Keyboard::F1:
					break;
				case sf::Keyboard::F2:
					break;
				case sf::Keyboard::F3:
					break;
				case sf::Keyboard::F4:
					break;
				case sf::Keyboard::F5:
					break;
				case sf::Keyboard::F6:
					break;
				case sf::Keyboard::F7:
					break;
				case sf::Keyboard::F8:
					break;
				case sf::Keyboard::F9:
					break;
				case sf::Keyboard::F10:
					break;
				case sf::Keyboard::F11:
					break;
				case sf::Keyboard::F12:
					break;
				case sf::Keyboard::F13:
					break;
				case sf::Keyboard::F14:
					break;
				case sf::Keyboard::F15:
					break;
				case sf::Keyboard::Pause:
					break;
				case sf::Keyboard::KeyCount:
					break;
			}
		}
	}
}

eDirection DisplaySfml::getDirection(void) const {
	return (direction_);
}

bool DisplaySfml::exit(void) const {
	return (exit_);
}

void
DisplaySfml::registerCallbackAction(std::function<void(eAction)> function) {
	callback_ = function;
}


DisplaySfml::SfmlConstructorException::SfmlConstructorException(void) noexcept :
		_error("Error on Sfml constructor") {}

DisplaySfml::SfmlConstructorException::SfmlConstructorException(
		std::string const &s) noexcept :
		_error(s) {}

const char *
DisplaySfml::SfmlConstructorException::what() const noexcept { return (_error.c_str()); }
