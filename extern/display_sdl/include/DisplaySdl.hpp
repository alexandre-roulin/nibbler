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

		virtual const char *what() const throw();

		~SdlConstructorException(void) throw();

		SdlConstructorException(SdlConstructorException const &src) throw();

	private:
		SdlConstructorException &
		operator=(SdlConstructorException const &rhs) throw();

		std::string _error;
	};

	DisplaySdl(int width,
			   int height,
			   char const *windowName);

	virtual ~DisplaySdl(void);

	bool exit(void) const override;
	void render(float currentDelayFrame, float maxDelayFram) override;
	void update(float delaTime) override;
	void drawGrid(Grid< eSprite > const &grid) override;
	void setBackground(Grid< eSprite > const &grid) override;
	eDirection getDirection(void) const override;

private:
	bool _exit;
	eDirection _direction;
	int _tileSize;
	Vector2D<int> const _winTileSize;
	Vector2D<int> const _winPixelSize;

	SDL_Window *_win;
	SDL_Surface *_rendererSurface;
	SDL_Texture *_rendererTexture;
	SDL_Renderer *_renderer;
	SDL_Surface *_tileset;
	SDL_Surface *_background;
	int _tilesetWidth;
	SDL_Event _ev;


	void _drawGrid(SDL_Surface *, Grid< eSprite > const &grid);

	void _drawTileGrid(int indexTile, int indexWidth, int indexHeight);
	void _drawTileGrid(SDL_Surface *, int indexWidthTile, int indexHeightTile,
					   int indexWidth, int indexHeight);

	void _drawTileGrid(SDL_Surface *, int indexTile, int indexWidth,
					   int indexHeight);

	SDL_Rect _getRectTile(int width, int height);

	SDL_Rect _getRectTilePixel(int width, int height);

	void _error(void);

	void _clean(void);

	DisplaySdl &operator=(DisplaySdl const &rhs);

	DisplaySdl(DisplaySdl const &src);

	DisplaySdl(void);

	static SDL_Surface *_SdlSurface(int widht, int height);
};

extern "C" {
IDisplay *newDisplay(int width,
					 int height,
					 char const *windowName);
void deleteDisplay(IDisplay *display);
}

