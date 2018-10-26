#pragma once
#include <SFML/Graphics.hpp>

#include <exception>
#include "IDisplay.hpp"
#include "Vector2D.tpp"
#include "Grid.tpp"

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
/*
	void		drawColorTile(int indexWidth, int indexHeight, int color);
	void		drawTile(int indexTile, int indexWidth, int indexHeight);
	void		drawTile(int indexWidthTile, int indexHeightTile, int indexWidth, int indexHeight);
	void		drawColorTile(SDL_Surface *, int indexWidth, int indexHeight, int color);
	void		drawTile(SDL_Surface *, int indexTile, int indexWidth, int indexHeight);
	void		drawTile(SDL_Surface *, int indexWidthTile, int indexHeightTile, int indexWidth, int indexHeight);

	void		drawGrid(Grid<int> const &grid);
	void		drawGrid(SDL_Surface *, Grid<int> const &grid);

	void		setBackground(Grid<int> const &grid);
*/
private:
    bool					_exit;
	int						_tileSize;
	Vector2D<int> const		_winTileSize;
    Vector2D<int> const		_winPixelSize;

    sf::RenderWindow		_win;

    sf::Texture				_tileset;
    sf::Texture				_background;

	int						_tilesetWidth;
    sf::Event               _ev;

    void                _error(std::string);
    void                _clean(void);

    DisplaySfml          &operator=(DisplaySfml const &rhs);
    DisplaySfml(DisplaySfml const &src);
    DisplaySfml(void);
};
