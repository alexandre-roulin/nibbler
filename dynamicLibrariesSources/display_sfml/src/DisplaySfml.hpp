#pragma once

#include <SFML/Graphics.hpp>

#include <exception>
#include "IDisplay.hpp"
#include "Vector2D.tpp"

class DisplaySfml : public IDisplay {
public:
	class SfmlConstructorException : public std::exception {
	public:
		SfmlConstructorException(void) noexcept;

		SfmlConstructorException(std::string const &s) noexcept;

		const char *what() const noexcept override;

		~SfmlConstructorException(void) noexcept = default;

	private:
		SfmlConstructorException &
		operator=(SfmlConstructorException const &rhs) noexcept = delete;

		SfmlConstructorException(SfmlConstructorException const &src) noexcept = delete;

		std::string _error;
	};

	DisplaySfml(int width,
				int height,
				char const *windowName);

	~DisplaySfml(void) override;

	void registerCallbackAction(std::function<void(eAction)> function) override; // TODO


	bool exit(void) const override;

	void render(float, float) override;

	void update(float) override;

	void drawGrid(MutantGrid<eSprite> const &grid) override;

	void setBackground(MutantGrid<eSprite> const &grid) override;

	eDirection getDirection(void) const override;

private:
	std::function<void(eAction)> callback_;
	bool exit_;
	eDirection direction_;
	int tileSize_;
	Vector2D<int> const winTileSize_;
	Vector2D<int> const winPixelSize_;

	sf::RenderWindow win_;

	sf::Texture tileset_;
	sf::RenderTexture textureBackground_;
	sf::Sprite spriteBackground_;


	int tilesetWidth_;
	sf::Event ev_;

	void drawTileGrid_(int indexTile, int indexWidthGrid, int indexHeightGrid);

	void drawGrid_(sf::RenderTarget &target, MutantGrid<eSprite> const &grid);

	void drawTileGrid_(sf::RenderTarget &target, int indexTile, int indexWidthGrid,
					   int indexHeightGrid);

	sf::VertexArray getQuadTilePixel_(int indexWidthTile, int indexHeightTile,
									  int indexWidthGrid, int indexHeightGrid);

	void _error(std::string const &s);

	void _clean(void);


	DisplaySfml &operator=(DisplaySfml const &rhs) = delete;

	DisplaySfml(DisplaySfml const &src) = delete;

	DisplaySfml(void) = delete;
};

extern "C" {
IDisplay *newDisplay(int width,
					 int height,
					 char const *windowName);
void deleteDisplay(IDisplay *display);
}
