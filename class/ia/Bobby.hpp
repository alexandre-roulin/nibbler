#pragma once

#include <nibbler.hpp>
#include <Grid.tpp>
#include <map>
#include "AStar.hpp"

class Univers;

class Bobby  {
private:
	std::mutex mutex;
	eDirection direction;
	AStar::Generator generator;
	Univers &univers_;
	unsigned int mapSize;
	unsigned int baseIndex;

	void findDirection(AStar::Vec2i, AStar::CoordinateList);
	AStar::Vec2i getVecSnakeHead();
	AStar::Vec2i getVecSnakeTail();
	AStar::Vec2i getVecFood(AStar::Vec2i head);
	void addCollision();
public:

	void calculateDirection();
	Bobby(Univers &univers);
	eDirection getDirection();
};
