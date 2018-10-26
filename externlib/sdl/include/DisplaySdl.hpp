#pragma once

#include <exception>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "IDisplay.hpp"
#include "Vector2D.tpp"
#include "Grid.tpp"

class DisplaySdl : public IDisplay {
    public:

		class SdlConstructorException : public std::exception {
		public:
			SdlConstructorException(void) throw();
			SdlConstructorException(std::string) throw();
			virtual const char* what() const throw();
			~SdlConstructorException(void) throw();
			SdlConstructorException(SdlConstructorException const &src) throw();
		private:
			SdlConstructorException &operator=(SdlConstructorException const &rhs) throw();
			std::string			_error;
		};

    DisplaySdl(char const *tileset,
				int tile,
				int width,
				int height,
				char const * windowName);
    virtual ~DisplaySdl(void);

    bool        exit(void) const;
    void        render(void);
    void        update(void);

	void		drawColorTile(int indexWidth, int indexHeight, int color);
	void		drawTile(int indexTile, int indexWidth, int indexHeight);
	void		drawTile(int indexWidthTile, int indexHeightTile, int indexWidth, int indexHeight);
	void		drawColorTile(SDL_Surface *, int indexWidth, int indexHeight, int color);
	void		drawTile(SDL_Surface *, int indexTile, int indexWidth, int indexHeight);
	void		drawTile(SDL_Surface *, int indexWidthTile, int indexHeightTile, int indexWidth, int indexHeight);

	void		drawGrid(Grid<int> const &grid);
	void		drawGrid(SDL_Surface *, Grid<int> const &grid);

	void		setBackground(Grid<int> const &grid);
private:
    bool					_exit;
	int						_tileSize;
	Vector2D<int> const		_winTileSize;
    Vector2D<int> const		_winPixelSize;

    SDL_Window				*_win;
    SDL_Surface				*_rendererSurface;
    SDL_Texture				*_rendererTexture;
    SDL_Renderer			*_renderer;
	SDL_Surface				*_tileset;
	SDL_Surface				*_background;
	int						_tilesetWidth;
    SDL_Event				_ev;

    void                _error(void);
    void                _clean(void);

    DisplaySdl          &operator=(DisplaySdl const &rhs);
    DisplaySdl(DisplaySdl const &src);
    DisplaySdl(void);

	static SDL_Surface * _SdlSurface(int widht, int height);
};
