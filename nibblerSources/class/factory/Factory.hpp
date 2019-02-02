#pragma once

#include <nibbler.hpp>

class Univers;

class Factory {
public:
	void create_all_snake(std::array<Snake, 8> snake_array, int16_t nu);

	void create_snake(Snake, int);

	static char *factory_chat_message(char const *name, char const *message);

	Factory(Univers &univers);

private:
	void create_walls();

	void create_wall(int x, int y);
	Univers &univers_;
};
