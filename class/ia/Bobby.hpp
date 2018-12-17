#pragma once

#include <nibbler.hpp>
#include <Grid.tpp>
#include <map>
#include <network/ClientTCP.hpp>
#include "AStar.hpp"


class Univers;

class Bobby {
private:

	std::mutex mutex;
	eDirection direction;
	AStar::Generator generator;
	Univers &univers_;
	unsigned int mapSize;
	unsigned int baseIndex;
	Grid<int> const * grid_;
public:
	void setGrid_(const Grid<int> *grid_);

	uint16_t getId() const;
private:
	ClientTCP::pointer_client clientTCP_;
	void findDirection(AStar::Vec2i, AStar::CoordinateList);

	AStar::Vec2i getVecSnakeHead();

	AStar::Vec2i getVecSnakeTail();

	AStar::Vec2i getVecFood(AStar::Vec2i head);

	void addCollision();

public:

	Bobby(Univers &, ClientTCP::pointer_client);

	void buildIA();
	void sendDirection() ;
	void calculateDirection();

};
