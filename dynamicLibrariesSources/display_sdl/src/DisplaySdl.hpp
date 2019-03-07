#pragma once

#include <exception>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "IDisplay.hpp"
#include "Vector2D.tpp"
#include "MutantGrid.tpp"

class DisplaySdl : public IDisplay {
public:

	class SdlConstructorException : public std::exception {
	public:
		SdlConstructorException(void) noexcept;

		SdlConstructorException(std::string const &) noexcept;

		const char *what() const noexcept override;

		~SdlConstructorException(void) noexcept = default;

	private:
		SdlConstructorException &
		operator=(SdlConstructorException const &rhs) noexcept = delete;

		SdlConstructorException(SdlConstructorException const &src) noexcept = delete;

		std::string error_;
	};

	DisplaySdl(int width,
			   int height,
			   char const *windowName);

	~DisplaySdl(void) override;

	bool exit(void) const override;

	void render() override;

	void update() override;

	void drawGrid(MutantGrid<eSprite> const &grid) override;

	void setBackground(MutantGrid<eSprite> const &grid) override;

	eDirection getDirection(void) const override;

	void registerCallbackAction(std::function<void(eAction)> function) override;

private:
	bool exit_;
	eDirection direction_;
	int tileSize_;
	Vector2D<int> const winTileSize_;
	Vector2D<int> const winPixelSize_;

	SDL_Window *win_;
	SDL_Surface *rendererSurface_;
	SDL_Texture *rendererTexture_;
	SDL_Renderer *renderer_;
	SDL_Surface *tileset_;
	SDL_Surface *background_;
	int tilesetWidth_;
	SDL_Event ev_;
	std::function<void(eAction)> callback_;


	void drawGrid_(SDL_Surface *surface, MutantGrid<eSprite> const &grid);

	void drawTileGrid_(int indexTile, int indexWidthGrid, int indexHeightGrid);

	void drawTileGrid_(SDL_Surface *surface, int indexTile, int indexWidthGrid,
					   int indexHeightGrid);

	SDL_Rect getRectTile_(int width, int height);

	void error_(void);

	void clean_(void);

	DisplaySdl &operator=(DisplaySdl const &rhs) = delete;

	DisplaySdl(DisplaySdl const &src) = delete;

	DisplaySdl(void) = delete;

	static SDL_Surface *sdlSurface_(int widht, int height);

};

extern "C" {
IDisplay *newInstance(int width,
					 int height,
					 char const *windowName);
void deleteInstance(IDisplay *display);
}

