#include <nibbler.hpp>
#include "DisplayMinilibx.hpp"

IDisplay2 *newDisplay(char const *tileset,
					 int tileSize,
					 int width,
					 int height,
					 char const *windowName) {
	return (new DisplayMinilibx(tileset, tileSize, width, height, windowName));
}

void deleteDisplay(IDisplay2 *display) {
	delete display;
}

DisplayMinilibx::DisplayMinilibx(char const *tileset,
						 int tileSize,
						 int width,
						 int height,
						 char const *windowName) :
		_exit(false),
		_direction(NORTH),
		_tileSize(tileSize),
		_winTileSize(Vector2D<int>(width, height)),
		_winPixelSize(Vector2D<int>(width * tileSize, height * tileSize)),
		_mlx(nullptr),
		_win(nullptr),
		_img(nullptr),
		_data(nullptr)
{
/*	if (!this->_tileset.loadFromFile(tileset))
		this->_error("Tileset cannot be load");
	if (!this->_textureBackground.create(this->_winPixelSize.getX(),
										 this->_winPixelSize.getY()))
		this->_error("Error during creation of Texture background");
	this->_tilesetWidth = (this->_tileset.getSize().x / this->_tileSize);
	this->_win.clear();
	this->_textureBackground.clear();
	this->_win.display();
*/

	this->_mlx = mlx_init();
	if (!(this->_img = mlx_new_image(this->_mlx, this->_winPixelSize.getX(), this->_winPixelSize.getY())))
	{
		this->_clean();
		throw (DisplayMinilibx::MinilibxConstructorException("mlxji_new_img: nullptr"));
	}
	if (!(this->_data = mlx_get_data_addr(this->_img, nullptr, nullptr, nullptr)))
	{
		this->_clean();
		throw (DisplayMinilibx::MinilibxConstructorException("mlx_get_data_addr: nullptr"));
	}
	if (!(this->_win = mlx_new_window(this->_mlx, this->_winPixelSize.getX(), this->_winPixelSize.getY(), const_cast< char * >(windowName))))
	{
		this->_clean();
		throw (DisplayMinilibx::MinilibxConstructorException("mlx_new_window: nullptr"));
	}
/*
	mlx_hook(this->_win, KEYPRESS, KEYPRESSMASK, &event_key_on, e);
	mlx_hook(this->_win, KEYRELEASE, KEYRELEASEMASK, &event_key_off, e);
	mlx_hook(this->_win, 17, (1 << 17), &event_red_cross, e);
*/
}

DisplayMinilibx::~DisplayMinilibx(void) {
	this->_clean();
}

void DisplayMinilibx::_clean(void) {
	if (this->_mlx)
	{
		if (this->_img)
			mlx_destroy_image(this->_mlx, this->_img);
		if (this->_mlx && this->_win)
			mlx_destroy_window(this->_mlx, this->_win);
		if (this->_data)
			delete [] this->_data;
	}
}

void DisplayMinilibx::render(void) {
	mlx_put_image_to_window(this->_mlx, this->_win, this->_img, 0, 0);

	//this->_win.draw(this->_spriteBackground);
}

void DisplayMinilibx::update(void) {
}

eDirection DisplayMinilibx::getDirection(void) const {
	return (this->_direction);
}

bool DisplayMinilibx::exit(void) const {
	return (this->_exit);
}

/*
**####################ID_TILE
*/
/*
void DisplayMinilibx::drawTileGrid(int indexTile, int indexWidthGrid,
							   int indexHeightGrid) {
	this->_win.draw(this->_getQuadTilePixel(indexTile % this->_tilesetWidth,
											indexTile / this->_tilesetWidth,
											indexWidthGrid * this->_tileSize,
											indexHeightGrid * this->_tileSize),
					&this->_tileset);
}

void DisplayMinilibx::_drawTileGrid(sf::RenderTarget &target, int indexTile,
								int indexWidthGrid, int indexHeightGrid) {
	target.draw(this->_getQuadTilePixel(indexTile % this->_tilesetWidth,
										indexTile / this->_tilesetWidth,
										indexWidthGrid * this->_tileSize,
										indexHeightGrid * this->_tileSize),
				&this->_tileset);
}

void DisplayMinilibx::drawTilePixel(int indexTile, int indexWidthPixel,
								int indexHeightPixel) {
	this->_win.draw(this->_getQuadTilePixel(indexTile % this->_tilesetWidth,
											indexTile / this->_tilesetWidth,
											indexWidthPixel,
											indexHeightPixel),
					&this->_tileset);
}
*/
/*
**####################INDEX_TILE X Y
*/
/*
void DisplayMinilibx::drawTileGrid(int indexWidthTile, int indexHeightTile,
							   int indexWidthGrid, int indexHeightGrid) {
	this->_win.draw(this->_getQuadTilePixel(indexWidthTile,
											indexHeightTile,
											indexWidthGrid * this->_tileSize,
											indexHeightGrid * this->_tileSize),
					&this->_tileset);
}

void DisplayMinilibx::_drawTileGrid(sf::RenderTarget &target,
								int indexWidthTile, int indexHeightTile,
								int indexWidthGrid, int indexHeightGrid) {
	target.draw(this->_getQuadTilePixel(indexWidthTile,
										indexHeightTile,
										indexWidthGrid * this->_tileSize,
										indexHeightGrid * this->_tileSize),
				&this->_tileset);
}

void DisplayMinilibx::drawTilePixel(int indexWidthTile, int indexHeightTile,
								int indexWidthPixel, int indexHeightPixel) {
	this->_win.draw(this->_getQuadTilePixel(indexWidthTile,
											indexHeightTile,
											indexWidthPixel,
											indexHeightPixel),
					&this->_tileset);
}
*/
/*
**####################DRAW_GRID
*/
/*
void DisplayMinilibx::drawGrid(Grid<int> const &grid) {
	for (int y = 0; y < this->_winTileSize.getY(); ++y)
		for (int x = 0; x < this->_winTileSize.getX(); ++x)
			if (grid(x, y) != -1)
				this->drawTileGrid(grid(x, y), x, y);
}

void DisplayMinilibx::_drawGrid(sf::RenderTarget &target, Grid<int> const &grid) {
	for (int y = 0; y < this->_winTileSize.getY(); ++y)
		for (int x = 0; x < this->_winTileSize.getX(); ++x)
			if (grid(x, y) != -1)
				this->_drawTileGrid(target, grid(x, y), x, y);
}

void DisplayMinilibx::setBackground(Grid<int> const &grid) {
	this->_drawGrid(this->_textureBackground, grid);
	this->_textureBackground.display();
	this->_spriteBackground = sf::Sprite(this->_textureBackground.getTexture());
}
*/

DisplayMinilibx::MinilibxConstructorException::~MinilibxConstructorException(
		void) throw() {}

DisplayMinilibx::MinilibxConstructorException::MinilibxConstructorException(void) throw() :
		_error("Error on Sfml constructor") {}

DisplayMinilibx::MinilibxConstructorException::MinilibxConstructorException(
		std::string s) throw() :
		_error(s) {}

DisplayMinilibx::MinilibxConstructorException::MinilibxConstructorException(
		DisplayMinilibx::MinilibxConstructorException const &src) throw() :
		_error(src._error) { this->_error = src._error; }

const char *
DisplayMinilibx::MinilibxConstructorException::what() const throw() { return (this->_error.c_str()); }
