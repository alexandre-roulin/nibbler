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

	void		drawTileGrid(int indexTile, int indexWidth, int indexHeight);
	void		drawTileGrid(int indexWidthTile, int indexHeightTile, int indexWidth, int indexHeight);
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

    SDL_Window				*_win;
    SDL_Surface				*_rendererSurface;
    SDL_Texture				*_rendererTexture;
    SDL_Renderer			*_renderer;
	SDL_Surface				*_tileset;
	SDL_Surface				*_background;
	int						_tilesetWidth;
    SDL_Event               _ev;

    void	                _drawGrid(SDL_Surface *, Grid<int> const &grid);
    void                    _drawTileGrid(SDL_Surface *, int indexWidthTile, int indexHeightTile, int indexWidth, int indexHeight);
    void	                _drawTileGrid(SDL_Surface *, int indexTile, int indexWidth, int indexHeight);

    SDL_Rect	            _getRectTile(int width, int height);
    SDL_Rect	            _getRectTilePixel(int width, int height);

    void                    _error(void);
    void                    _clean(void);

    DisplaySdl          &operator=(DisplaySdl const &rhs);
    DisplaySdl(DisplaySdl const &src);
    DisplaySdl(void);

	static SDL_Surface * _SdlSurface(int widht, int height);
};

extern "C" {
    IDisplay			*newDisplay(char const *tileset,
    						int tileSize,
    						int width,
    						int height,
    						char const *windowName);
    void				deleteDisplay(IDisplay *display);
}
