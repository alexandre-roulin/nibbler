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

		virtual const char *what() const throw();

		~SfmlConstructorException(void) throw();

		SfmlConstructorException(SfmlConstructorException const &src) throw();

	private:
		SfmlConstructorException &
		operator=(SfmlConstructorException const &rhs) throw();

		std::string _error;
	};

	DisplaySfml(int width,
				int height,
				char const *windowName);

	virtual ~DisplaySfml(void);

	bool exit(void) const;
	void render(void);
	void update(void);
	void drawGrid(Grid< eSprite > const &grid);
	void setBackground(Grid< eSprite > const &grid);
	eDirection getDirection(void) const;

private:
	bool _exit;
	eDirection _direction;
	int _tileSize;
	Vector2D<int> const _winTileSize;
	Vector2D<int> const _winPixelSize;

	sf::RenderWindow _win;

	sf::Texture _tileset;
	sf::RenderTexture _textureBackground;
	sf::Sprite _spriteBackground;


	int _tilesetWidth;
	sf::Event _ev;

	void _drawTileGrid(int indexTile, int indexWidthGrid, int indexHeightGrid);
	void _drawGrid(sf::RenderTarget &, Grid< eSprite > const &grid);
	void _drawTileGrid(sf::RenderTarget &, int indexTile, int indexWidthGrid,
					   int indexHeightGrid);
	void
	_drawTileGrid(sf::RenderTarget &, int indexWidthTile, int indexHeightTile,
				  int indexWidthGrid, int indexHeightGrid);

	sf::VertexArray _getQuadTilePixel(int indexWidthTile, int indexHeightTile,
									  int indexWidthGrid, int indexHeightGrid);

	void _error(std::string);
	void _clean(void);


	int			getSpriteSnake_(eSprite sprite);
	void		debugSpriteSnake_(eSprite sprite);
	int			direction_(eSprite to);


		DisplaySfml &operator=(DisplaySfml const &rhs);

	DisplaySfml(DisplaySfml const &src);

	DisplaySfml(void);
};

extern "C" {
IDisplay *newDisplay(int width,
					 int height,
					 char const *windowName);
void deleteDisplay(IDisplay *display);
}
