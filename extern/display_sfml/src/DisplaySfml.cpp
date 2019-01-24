#include <nibbler.hpp>
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
		_exit(false),
		_direction(kNorth),
		_tileSize(DISPLAY_DEFAULT_TILE_SIZE),
		_winTileSize(Vector2D<int>(width, height)),
		_winPixelSize(Vector2D<int>(width * _tileSize, height * _tileSize)),
		_win(sf::VideoMode(this->_winPixelSize.getX(),
						   this->_winPixelSize.getY()),
			 windowName) {
	std::cout << "DisplaySfml" << std::endl;

	this->_win.setFramerateLimit(60);

	std::string pathFile = __FILE__;
	std::string pathRoot = pathFile.substr(0, pathFile.rfind(DISPLAY_SLASH));
	pathRoot = pathFile.substr(0, pathRoot.rfind(DISPLAY_SLASH));

	if (!this->_tileset.loadFromFile(pathRoot + DISPLAY_SLASH + DISPLAY_DEFAULT_TILESET_PATH))
		this->_error("Tileset cannot be load");
	if (!this->_textureBackground.create(this->_winPixelSize.getX(),
										 this->_winPixelSize.getY()))
		this->_error("Error during creation of Texture background");
	this->_tilesetWidth = (this->_tileset.getSize().x / this->_tileSize);
	this->_win.clear();
	this->_textureBackground.clear();
	this->_win.display();
}

DisplaySfml::~DisplaySfml(void) {
	std::cout << "~DisplaySfml" << std::endl;
	this->_clean();
}

void DisplaySfml::_error(std::string s = std::string("Error")) {
	this->_clean();
	throw (DisplaySfml::SfmlConstructorException(s));
}

void DisplaySfml::_clean(void) {
	this->_win.close();
}

void DisplaySfml::render(float currentDelayFrame, float maxDelayFrame) {
	this->_win.display();
	this->_win.draw(this->_spriteBackground);
}

sf::VertexArray
DisplaySfml::_getQuadTilePixel(int indexWidthTile, int indexHeightTile,
							   int indexWidthPixel, int indexHeightPixel) {
	sf::VertexArray quad(sf::Quads, 4);
	quad[0].position = sf::Vector2f(((indexWidthPixel)), ((indexHeightPixel)));
	quad[1].position = sf::Vector2f(this->_tileSize, 0.f) + quad[0].position;

	quad[2].position = sf::Vector2f(((indexWidthPixel + this->_tileSize)),
									((indexHeightPixel + this->_tileSize)));
	quad[3].position = quad[2].position - sf::Vector2f(this->_tileSize, 0.f);

	quad[0].texCoords = sf::Vector2f((this->_tileSize) * (indexWidthTile),
									 (this->_tileSize) * (indexHeightTile));
	quad[1].texCoords = sf::Vector2f(this->_tileSize, 0.f) + quad[0].texCoords;

	quad[2].texCoords = sf::Vector2f((this->_tileSize) * (indexWidthTile + 1),
									 (this->_tileSize) * (indexHeightTile + 1));
	quad[3].texCoords = quad[2].texCoords - sf::Vector2f(this->_tileSize, 0.f);
	return (quad);
}

/*
**####################ID_TILE
*/

void DisplaySfml::_drawTileGrid(int indexTile, int indexWidthGrid,
							   int indexHeightGrid) {
	this->_win.draw(this->_getQuadTilePixel(indexTile % this->_tilesetWidth,
											indexTile / this->_tilesetWidth,
											indexWidthGrid * this->_tileSize,
											indexHeightGrid * this->_tileSize),
					&this->_tileset);
}

void DisplaySfml::_drawTileGrid(sf::RenderTarget &target, int indexTile,
								int indexWidthGrid, int indexHeightGrid) {
	target.draw(this->_getQuadTilePixel(indexTile % this->_tilesetWidth,
										indexTile / this->_tilesetWidth,
										indexWidthGrid * this->_tileSize,
										indexHeightGrid * this->_tileSize),
				&this->_tileset);
}

/*
**####################INDEX_TILE X Y
*/

void DisplaySfml::_drawTileGrid(sf::RenderTarget &target,
								int indexWidthTile, int indexHeightTile,
								int indexWidthGrid, int indexHeightGrid) {
	target.draw(this->_getQuadTilePixel(indexWidthTile,
										indexHeightTile,
										indexWidthGrid * this->_tileSize,
										indexHeightGrid * this->_tileSize),
				&this->_tileset);
}

/*
**####################DRAW_GRID
*/

void DisplaySfml::drawGrid(Grid< eSprite > const &grid) {
	for (int y = 0; y < this->_winTileSize.getY(); ++y)
		for (int x = 0; x < this->_winTileSize.getX(); ++x)
			if (grid(x, y) != eSprite::NONE)
				this->_drawTileGrid(Display2D::getSpriteSnake(grid(x, y)), x, y);
}

void DisplaySfml::_drawGrid(sf::RenderTarget &target, Grid< eSprite > const &grid) {
	for (int y = 0; y < this->_winTileSize.getY(); ++y)
		for (int x = 0; x < this->_winTileSize.getX(); ++x)
			if (grid(x, y) != eSprite::NONE)
				this->_drawTileGrid(target, Display2D::getSpriteSnake(grid(x, y)), x, y);
}

void DisplaySfml::setBackground(Grid< eSprite > const &grid) {
	this->_drawGrid(this->_textureBackground, grid);
	this->_textureBackground.display();
	this->_spriteBackground = sf::Sprite(this->_textureBackground.getTexture());
}

void DisplaySfml::update(float deltaTime) {
	while (_win.pollEvent(_ev)) {
		if (_ev.type == sf::Event::Closed)
			_exit = true;
		if (_ev.type == sf::Event::KeyPressed) {
			switch (_ev.key.code) {
				case sf::Keyboard::Unknown:break;
				case sf::Keyboard::A:
					_direction = kWest;
					break;
				case sf::Keyboard::B:break;
				case sf::Keyboard::C:break;
				case sf::Keyboard::D:
					_direction = kEast;
					break;
				case sf::Keyboard::E:break;
				case sf::Keyboard::F:break;
				case sf::Keyboard::G:break;
				case sf::Keyboard::H:break;
				case sf::Keyboard::I:break;
				case sf::Keyboard::J:break;
				case sf::Keyboard::K:break;
				case sf::Keyboard::L:break;
				case sf::Keyboard::M:break;
				case sf::Keyboard::N:break;
				case sf::Keyboard::O:
					_callback(eAction::kSwitchDisplayLibrary);
					break;
				case sf::Keyboard::P:
					_callback(eAction::kPause);
					break;
				case sf::Keyboard::Q:break;
				case sf::Keyboard::R:break;
				case sf::Keyboard::S:
					_direction = kSouth;
					break;
				case sf::Keyboard::T:break;
				case sf::Keyboard::U:break;
				case sf::Keyboard::V:break;
				case sf::Keyboard::W:
					_direction = kNorth;

					break;
				case sf::Keyboard::X:break;
				case sf::Keyboard::Y:break;
				case sf::Keyboard::Z:break;
				case sf::Keyboard::Num0:break;
				case sf::Keyboard::Num1:break;
				case sf::Keyboard::Num2:break;
				case sf::Keyboard::Num3:break;
				case sf::Keyboard::Num4:break;
				case sf::Keyboard::Num5:break;
				case sf::Keyboard::Num6:break;
				case sf::Keyboard::Num7:break;
				case sf::Keyboard::Num8:break;
				case sf::Keyboard::Num9:break;
				case sf::Keyboard::Escape:break;
				case sf::Keyboard::LControl:break;
				case sf::Keyboard::LShift:break;
				case sf::Keyboard::LAlt:break;
				case sf::Keyboard::LSystem:break;
				case sf::Keyboard::RControl:break;
				case sf::Keyboard::RShift:break;
				case sf::Keyboard::RAlt:break;
				case sf::Keyboard::RSystem:break;
				case sf::Keyboard::Menu:break;
				case sf::Keyboard::LBracket:break;
				case sf::Keyboard::RBracket:break;
				case sf::Keyboard::SemiColon:break;
				case sf::Keyboard::Comma:break;
				case sf::Keyboard::Period:break;
				case sf::Keyboard::Quote:break;
				case sf::Keyboard::Slash:break;
				case sf::Keyboard::BackSlash:break;
				case sf::Keyboard::Tilde:break;
				case sf::Keyboard::Equal:break;
				case sf::Keyboard::Dash:break;
				case sf::Keyboard::Space:break;
				case sf::Keyboard::Return:break;
				case sf::Keyboard::BackSpace:break;
				case sf::Keyboard::Tab:break;
				case sf::Keyboard::PageUp:break;
				case sf::Keyboard::PageDown:break;
				case sf::Keyboard::End:break;
				case sf::Keyboard::Home:break;
				case sf::Keyboard::Insert:break;
				case sf::Keyboard::Delete:break;
				case sf::Keyboard::Add:break;
				case sf::Keyboard::Subtract:break;
				case sf::Keyboard::Multiply:break;
				case sf::Keyboard::Divide:break;
				case sf::Keyboard::Left:
					_direction = kWest;
					break;
				case sf::Keyboard::Right:
					_direction = kEast;
					break;
				case sf::Keyboard::Up:
					_direction = kNorth;
					break;
				case sf::Keyboard::Down:
					_direction = kSouth;
					break;
				case sf::Keyboard::Numpad0:break;
				case sf::Keyboard::Numpad1:break;
				case sf::Keyboard::Numpad2:break;
				case sf::Keyboard::Numpad3:break;
				case sf::Keyboard::Numpad4:break;
				case sf::Keyboard::Numpad5:break;
				case sf::Keyboard::Numpad6:break;
				case sf::Keyboard::Numpad7:break;
				case sf::Keyboard::Numpad8:break;
				case sf::Keyboard::Numpad9:break;
				case sf::Keyboard::F1:break;
				case sf::Keyboard::F2:break;
				case sf::Keyboard::F3:break;
				case sf::Keyboard::F4:break;
				case sf::Keyboard::F5:break;
				case sf::Keyboard::F6:break;
				case sf::Keyboard::F7:break;
				case sf::Keyboard::F8:break;
				case sf::Keyboard::F9:break;
				case sf::Keyboard::F10:break;
				case sf::Keyboard::F11:break;
				case sf::Keyboard::F12:break;
				case sf::Keyboard::F13:break;
				case sf::Keyboard::F14:break;
				case sf::Keyboard::F15:break;
				case sf::Keyboard::Pause:break;
				case sf::Keyboard::KeyCount:break;
			}
		}
	}
}

eDirection DisplaySfml::getDirection(void) const {
	return (this->_direction);
}

bool DisplaySfml::exit(void) const {
	return (this->_exit);
}

void
DisplaySfml::registerCallbackAction(std::function<void(eAction)> function) {
	_callback = function;
}


DisplaySfml::SfmlConstructorException::~SfmlConstructorException(
		void) throw() {}

DisplaySfml::SfmlConstructorException::SfmlConstructorException(void) throw() :
		_error("Error on Sfml constructor") {}

DisplaySfml::SfmlConstructorException::SfmlConstructorException(
		std::string s) throw() :
		_error(s) {}

DisplaySfml::SfmlConstructorException::SfmlConstructorException(
		DisplaySfml::SfmlConstructorException const &src) throw() :
		_error(src._error) { this->_error = src._error; }

const char *
DisplaySfml::SfmlConstructorException::what() const throw() { return (this->_error.c_str()); }
