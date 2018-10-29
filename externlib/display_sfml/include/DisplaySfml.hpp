#pragma once
#include <SFML/Graphics.hpp>

#include <exception>
#include "IDisplay.hpp"
#include "Vector2D.tpp"

class DisplaySfml : public IDisplay {
    public:

		class SfmlConstructorException : public std::exception {
		public:
			SfmlConstructorException(void) throw();
			SfmlConstructorException(std::string) throw();
			virtual const char* what() const throw();
			~SfmlConstructorException(void) throw();
			SfmlConstructorException(SfmlConstructorException const &src) throw();
		private:
			SfmlConstructorException &operator=(SfmlConstructorException const &rhs) throw();
			std::string			_error;
		};

    DisplaySfml(char const *tileset,
				int tile,
				int width,
				int height,
				char const * windowName);
    virtual ~DisplaySfml(void);

    bool        exit(void) const;
    void        render(void);
    void        update(void);

	void		drawTileGrid(int indexTile, int indexWidthGrid, int indexHeightGrid);
	void		drawTileGrid(int indexWidthTile, int indexHeightTile, int indexWidthGrid, int indexHeightGrid);
    void		drawTilePixel(int indexTile, int indexWidthPixel, int indexHeightPixel);
	void		drawTilePixel(int indexWidthTile, int indexHeightTile, int indexWidthPixel, int indexHeightPixel);
	void		drawGrid(Grid<int> const &grid);
	void		setBackground(Grid<int> const &grid);

    eDirection  getDirection(void) const;
private:
    bool					_exit;
    eDirection              _direction;
	int						_tileSize;
	Vector2D<int> const		_winTileSize;
    Vector2D<int> const		_winPixelSize;

    sf::RenderWindow		_win;

    sf::Texture				_tileset;
    sf::RenderTexture       _textureBackground;
    sf::Sprite              _spriteBackground;


	int						_tilesetWidth;
    sf::Event               _ev;

    void		        _drawGrid(sf::RenderTarget &, Grid<int> const &grid);
    void		        _drawTileGrid(sf::RenderTarget &, int indexTile, int indexWidthGrid, int indexHeightGrid);
	void		        _drawTileGrid(sf::RenderTarget &, int indexWidthTile, int indexHeightTile, int indexWidthGrid, int indexHeightGrid);

    sf::VertexArray		_getQuadTilePixel(int indexWidthTile, int indexHeightTile, int indexWidthGrid, int indexHeightGrid);

    void                _error(std::string);
    void                _clean(void);


    DisplaySfml          &operator=(DisplaySfml const &rhs);
    DisplaySfml(DisplaySfml const &src);
    DisplaySfml(void);
};

extern "C" {
    IDisplay			*newDisplay(char const *tileset,
    						int tileSize,
    						int width,
    						int height,
    						char const *windowName);
    void				deleteDisplay(IDisplay *display);
}