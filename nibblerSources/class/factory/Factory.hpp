#pragma once

#include <nibbler.hpp>

class Univers;

class Factory {
public:
	void create_all_snake(std::array<Snake, SNAKE_MAX> snake_array, int16_t nu);

	void create_snake(Snake const &, int);

	Factory(Univers &univers);

private:
	void create_walls();

	void create_wall(int x, int y);
	Univers &univers_;
};
