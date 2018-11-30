#pragma once

#include <mlx.h>

#include <exception>
#include "IDisplay.hpp"
#include "Vector2D.tpp"

class IDisplay2 {
public:
	virtual ~IDisplay2() {}
    virtual bool        exit(void) const = 0;
    virtual void        render(void) = 0;
    virtual void        update(void) = 0;
    virtual eDirection  getDirection(void) const = 0;
};

class DisplayMinilibx : public IDisplay2 {
public:

	class MinilibxConstructorException : public std::exception {
	public:
		MinilibxConstructorException(void) throw();

		MinilibxConstructorException(std::string) throw();

		virtual const char *what() const throw();

		~MinilibxConstructorException(void) throw();

		MinilibxConstructorException(MinilibxConstructorException const &src) throw();

	private:
		MinilibxConstructorException &
		operator=(MinilibxConstructorException const &rhs) throw();

		std::string _error;
	};

	DisplayMinilibx(char const *tileset,
				int tile,
				int width,
				int height,
				char const *windowName);

	virtual ~DisplayMinilibx(void);

	bool exit(void) const;

	void render(void);

	void update(void);
/*
	void drawTileGrid(int indexTile, int indexWidthGrid, int indexHeightGrid);

	void
	drawTileGrid(int indexWidthTile, int indexHeightTile, int indexWidthGrid,
				 int indexHeightGrid);

	void
	drawTilePixel(int indexTile, int indexWidthPixel, int indexHeightPixel);

	void
	drawTilePixel(int indexWidthTile, int indexHeightTile, int indexWidthPixel,
				  int indexHeightPixel);

	void drawGrid(Grid<int> const &grid);

	void setBackground(Grid<int> const &grid);
*/
	eDirection getDirection(void) const;

private:
	bool					_exit;
	eDirection				_direction;
	int						_tileSize;
	Vector2D<int> const		_winTileSize;
	Vector2D<int> const		_winPixelSize;

	void					*_mlx;
	void					*_win;
	void					*_img;
	char					*_data;

	int						_tilesetWidth;
/*
	void _drawGrid(sf::RenderTarget &, Grid<int> const &grid);

	void _drawTileGrid(sf::RenderTarget &, int indexTile, int indexWidthGrid,
					   int indexHeightGrid);

	void
	_drawTileGrid(sf::RenderTarget &, int indexWidthTile, int indexHeightTile,
				  int indexWidthGrid, int indexHeightGrid);
*/
	void _error(std::string);

	void _clean(void);


	DisplayMinilibx &operator=(DisplayMinilibx const &rhs);

	DisplayMinilibx(DisplayMinilibx const &src);

	DisplayMinilibx(void);
};

extern "C" {
IDisplay2 *newDisplay(char const *tileset,
					 int tileSize,
					 int width,
					 int height,
					 char const *windowName);
void deleteDisplay(IDisplay2 *display);
}
