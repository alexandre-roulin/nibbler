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
	void findDirection(AStar::Vec2i, AStar::CoordinateList);
public:
	void calculateDirection();
	Bobby(Univers &univers);
	eDirection getDirection();
};
