#pragma once

#include <nibbler.hpp>
#include <cores/Snake.hpp>

class Univers;

class Factory {
public:
	Factory(Univers &univers);
	Factory() = delete;
	~Factory() = default;
	Factory &operator=(const Factory &) = delete;
	Factory(const Factory &) = delete;

	void createAllSnake(std::shared_ptr<SnakeArrayContainer> snake_array, int16_t nu);

private:

	void createSnake(Snake const &snake, int maxSnakes, size_t n);

	void createWalls();

	void createWall(int x, int y);

	Univers &univers_;
	MutantGrid<bool> tempGrid;
};
