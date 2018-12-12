#include <nibbler.hpp>
#include "DisplaySfml.hpp"

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
		_direction(NORTH),
		_tileSize(DISPLAY_DEFAULT_TILE_SIZE),
		_winTileSize(Vector2D<int>(width, height)),
		_winPixelSize(Vector2D<int>(width * _tileSize, height * _tileSize)),
		_win(sf::VideoMode(this->_winPixelSize.getX(),
						   this->_winPixelSize.getY()),
			 windowName) {
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
	this->_clean();
}

void DisplaySfml::_error(std::string s = std::string("Error")) {
	this->_clean();
	throw (DisplaySfml::SfmlConstructorException(s));
}

void DisplaySfml::_clean(void) {
	this->_win.close();
}

void DisplaySfml::render(void) {
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

int DisplaySfml::direction_(eSprite to) {
	if ((to & eSprite::SOUTH) == eSprite::SOUTH)
		return (0);
	if ((to & eSprite::WEST) == eSprite::WEST)
		return (1);
	if ((to & eSprite::NORTH) == eSprite::NORTH)
		return (2);
	return (3);
}

void DisplaySfml::debugSpriteSnake_(eSprite sprite) {
	eSprite from = (sprite & eSprite::MASK_FROM) >> eSprite::BITWISE_FROM;
	eSprite to = (sprite & eSprite::MASK_TO) >> eSprite::BITWISE_TO;

	std::cout << "Entity : [";

	if ((sprite & eSprite::MASK_BODY) == eSprite::HEAD)
		std::cout << "HEAD";
	if ((sprite & eSprite::MASK_BODY) == eSprite::BODY)
		std::cout << "BODY";
	if ((sprite & eSprite::MASK_BODY) == eSprite::TAIL)
		std::cout << "TAIL";

	std::cout << "] FROM [";
	if ((from & eSprite::NORTH) == eSprite::NORTH)
		std::cout << "NORTH";
	if ((from & eSprite::SOUTH) == eSprite::SOUTH)
		std::cout << "SOUTH";
	if ((from & eSprite::WEST) == eSprite::WEST)
		std::cout << "WEST";
	if ((from & eSprite::EAST) == eSprite::EAST)
		std::cout << "EAST";
	std::cout << "]TO [";
	if ((to & eSprite::NORTH) == eSprite::NORTH)
		std::cout << "NORTH";
	if ((to & eSprite::SOUTH) == eSprite::SOUTH)
		std::cout << "SOUTH";
	if ((to & eSprite::WEST) == eSprite::WEST)
		std::cout << "WEST";
	if ((to & eSprite::EAST) == eSprite::EAST)
		std::cout << "EAST";
	std::bitset<32> c(static_cast<int>(sprite));

	std::cout << "::  : [" << c << "]" << std::endl;
}


int DisplaySfml::getSpriteSnake_(eSprite sprite) {

	eSprite from = (sprite & eSprite::MASK_FROM) >> eSprite::BITWISE_FROM;
	eSprite to = (sprite & eSprite::MASK_TO) >> eSprite::BITWISE_TO;

	if ((sprite & eSprite::WALL) == eSprite::WALL)
		return (SPRITE_WALL);
	if ((sprite & eSprite::GROUND) == eSprite::GROUND)
		return (SPRITE_GROUND);
	if ((sprite & eSprite::FOOD) == eSprite::FOOD)
		return (SPRITE_FOOD);
	if ((sprite & eSprite::MASK_BODY) == eSprite::HEAD)
		return (SIZE_LINE_TILESET *
			(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 5 +
				DisplaySfml::direction_(to));
	if ((sprite & eSprite::MASK_BODY) == eSprite::BODY) {

		if (from == to && (from == eSprite::NORTH || from == eSprite::SOUTH))
			return (SIZE_LINE_TILESET *
				(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 14);
		else if (from == to && (from == eSprite::EAST || from == eSprite::WEST))
			return (SIZE_LINE_TILESET *
				(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 13);

		else if (from == eSprite::NORTH && to == eSprite::EAST)
			return (SIZE_LINE_TILESET *
				(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 1);
		else if (from == eSprite::NORTH && to == eSprite::WEST)
			return (SIZE_LINE_TILESET *
				(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 2);

		else if (from == eSprite::SOUTH && to == eSprite::EAST)
			return (SIZE_LINE_TILESET *
				(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 3);
		else if (from == eSprite::SOUTH && to == eSprite::WEST)
			return (SIZE_LINE_TILESET *
				(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 4);

		else if (from == eSprite::WEST && to == eSprite::SOUTH)
			return (SIZE_LINE_TILESET *
				(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 1);
		else if (from == eSprite::EAST && to == eSprite::SOUTH)
			return (SIZE_LINE_TILESET *
				(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 2);

		else if (from == eSprite::WEST && to == eSprite::NORTH)
			return (SIZE_LINE_TILESET *
				(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 3);
		else if (from == eSprite::EAST && to == eSprite::NORTH)
			return (SIZE_LINE_TILESET *
				(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 4);

		else
			return (SIZE_LINE_TILESET);
	}
	if ((sprite & eSprite::MASK_BODY) == eSprite::TAIL)
		return (SIZE_LINE_TILESET *
			(static_cast<int>(sprite & eSprite::MASK_COLOR) - 1) + 9 +
				DisplaySfml::direction_(to));

	return (0);
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
				this->_drawTileGrid(DisplaySfml::getSpriteSnake_(grid(x, y)), x, y);
}

void DisplaySfml::_drawGrid(sf::RenderTarget &target, Grid< eSprite > const &grid) {
	for (int y = 0; y < this->_winTileSize.getY(); ++y)
		for (int x = 0; x < this->_winTileSize.getX(); ++x)
			if (grid(x, y) != eSprite::NONE)
				this->_drawTileGrid(target, DisplaySfml::getSpriteSnake_(grid(x, y)), x, y);
}

void DisplaySfml::setBackground(Grid< eSprite > const &grid) {
	this->_drawGrid(this->_textureBackground, grid);
	this->_textureBackground.display();
	this->_spriteBackground = sf::Sprite(this->_textureBackground.getTexture());
}

void DisplaySfml::update(void) {
	while (this->_win.pollEvent(this->_ev)) {
		if (this->_ev.type == sf::Event::Closed)
			this->_exit = true;
		if (this->_ev.type == sf::Event::KeyPressed) {
			if (this->_ev.key.code == sf::Keyboard::A)
				this->_direction = WEST;
			else if (this->_ev.key.code == sf::Keyboard::D)
				this->_direction = EAST;
			else if (this->_ev.key.code == sf::Keyboard::W)
				this->_direction = NORTH;
			else if (this->_ev.key.code == sf::Keyboard::S)
				this->_direction = SOUTH;
		}
	}
}

eDirection DisplaySfml::getDirection(void) const {
	return (this->_direction);
}

bool DisplaySfml::exit(void) const {
	return (this->_exit);
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
