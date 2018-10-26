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


    if (!this->_tileset.loadFromFile(tileset))
        this->_error("Tileset cannot be load");
	if (!this->_textureBackground.create(this->_winPixelSize.getX(), this->_winPixelSize.getY()))
		this->_error("Error during creation of Texture background");
	this->_tilesetWidth = (this->_tileset.getSize().x / this->_tileSize);
	this->_win.clear();
	this->_textureBackground.clear();
	this->_win.display();
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
	this->_win.draw(this->_spriteBackground);
}

sf::VertexArray		DisplaySfml::_getQuadTile(int indexWidthTile, int indexHeightTile,
										int indexWidthGrid, int indexHeightGrid)
{
	sf::VertexArray		quad(sf::Quads, 4);
	quad[0].position = sf::Vector2f(((indexWidthGrid) * this->_tileSize),
								((indexHeightGrid) * this->_tileSize));
	quad[1].position = sf::Vector2f(this->_tileSize, 0.f) + quad[0].position;

	quad[2].position = sf::Vector2f(((indexWidthGrid + 1) * this->_tileSize),
								((indexHeightGrid + 1) * this->_tileSize));
	quad[3].position = quad[2].position - sf::Vector2f(this->_tileSize, 0.f);

	quad[0].texCoords = sf::Vector2f((this->_tileSize) * (indexWidthTile),
								(this->_tileSize) * (indexHeightTile));
	quad[1].texCoords = sf::Vector2f(this->_tileSize, 0.f) + quad[0].texCoords;

	quad[2].texCoords = sf::Vector2f((this->_tileSize) * (indexWidthTile + 1),
								(this->_tileSize) * (indexHeightTile + 1));
	quad[3].texCoords = quad[2].texCoords - sf::Vector2f(this->_tileSize, 0.f);
	return (quad);
}

void			DisplaySfml::drawTile(int indexTile,
										int indexWidthGrid, int indexHeightGrid)
{
	this->_win.draw( this->_getQuadTile(indexTile % this->_tilesetWidth, indexTile / this->_tilesetWidth, indexWidthGrid, indexHeightGrid), &this->_tileset);
}

void			DisplaySfml::_drawTile(sf::RenderTarget &target,
										int indexTile,
										int indexWidthGrid, int indexHeightGrid)
{
	target.draw( this->_getQuadTile(indexTile % this->_tilesetWidth, indexTile / this->_tilesetWidth, indexWidthGrid, indexHeightGrid), &this->_tileset);
}

void			DisplaySfml::drawTile(int indexWidthTile, int indexHeightTile,
										int indexWidthGrid, int indexHeightGrid)
{
	this->_win.draw(this->_getQuadTile(indexWidthTile, indexHeightTile, indexWidthGrid, indexHeightGrid), &this->_tileset);
}
void			DisplaySfml::_drawTile(sf::RenderTarget &target,
										int indexWidthTile, int indexHeightTile,
										int indexWidthGrid, int indexHeightGrid)
{
	target.draw( this->_getQuadTile(indexWidthTile, indexHeightTile, indexWidthGrid, indexHeightGrid), &this->_tileset);
}

void			DisplaySfml::drawGrid(Grid<int> const &grid)
{
	for (int y = 0; y < this->_winTileSize.getY(); y++)
		for (int x = 0; x < this->_winTileSize.getX(); x++)
			this->drawTile(grid(x, y), x, y);
}
void			DisplaySfml::drawGrid(sf::RenderTarget &target, Grid<int> const &grid)
{
	for (int y = 0; y < this->_winTileSize.getY(); y++)
		for (int x = 0; x < this->_winTileSize.getX(); x++)
			this->_drawTile(target, grid(x, y), x, y);
}

void		DisplaySfml::setBackground(Grid<int> const &grid)
{
	this->drawGrid(this->_textureBackground, grid);
	this->_textureBackground.display();
	this->_spriteBackground = sf::Sprite(this->_textureBackground.getTexture());
}

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
