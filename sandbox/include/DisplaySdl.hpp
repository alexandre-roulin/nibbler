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
					unsigned int tile,
					unsigned int width,
					unsigned int height,
					char const * windowName);
        virtual ~DisplaySdl(void);

        bool        exit(void) const;
        void        render(void);
        void        update(void);

		void		drawColorTile(unsigned int indexWidth, unsigned int indexHeight, int color);
		void		drawTile(unsigned indexTile, unsigned int indexWidth, unsigned int indexHeight);
		void		drawTile(unsigned indexWidthTile, unsigned indexHeightTile, unsigned int indexWidth, unsigned int indexHeight);
		void		drawGrid(Grid<int> const &grid);

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
		int						_tilesetWidth;
        SDL_Event				_ev;

        void                _error(void);
        void                _clean(void);

        DisplaySdl          &operator=(DisplaySdl const &rhs);
        DisplaySdl(DisplaySdl const &src);
        DisplaySdl(void);

		static SDL_Surface * _SdlSurface(unsigned int widht, unsigned int height);
};
