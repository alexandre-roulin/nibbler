#pragma once

#include <nibbler.hpp>
#include <map>
#include <network/ClientTCP.hpp>
#include <KINU/Entity.hpp>
#include "AStar.hpp"


class Univers;

class Bobby {
private:

	enum ePriority {
		UNDEFINED,		//0
		NO_PRIORITY,	//1
		PRIORITY		//2
	};

	static std::unordered_map<KINU::Entity::ID, ePriority> mapPriority;

	eDirection direction;
	AStar::Generator generator;
	Univers &univers_;
	unsigned int mapSize;
	unsigned int baseIndex;
public:
	static std::mutex mutex;
	static void clearPriority();
	uint16_t getId() const;
	const ClientTCP::pointer_client &getClientTCP_() const;
	ClientTCP::pointer_client clientTCP_;
	void findDirection(AStar::Vec2i, AStar::CoordinateList);
	bool define_priority(int x, int y);
	AStar::Vec2i getVecSnakeHead();
	void findAnyDirectionValid(AStar::Vec2i );
	AStar::Vec2i getVecSnakeTail();



	AStar::Vec2i getVecFood(AStar::Vec2i head);

	void addCollision();


public:

	Bobby(Univers &, ClientTCP::pointer_client);

	void buildIA();
	void sendDirection() ;
	void calculateDirection();

};
