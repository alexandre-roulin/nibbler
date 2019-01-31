#pragma once

#include <nibbler.hpp>
#include <map>
#include <network/ClientTCP.hpp>
#include <KINU/Entity.hpp>
#include "KStar.hpp"


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
	Univers &univers_;
	unsigned int mapSize;
	unsigned int baseIndex;
	static std::mutex mutex;
	KStar kStar;
private:

	std::unique_ptr<ClientTCP> clientTCP_;
	void findDirection(KStar::Vec2 vecSource, KStar::Vec2 vecTarget);
	bool define_priority(int x, int y);
	KStar::Vec2 getVecSnakeTail();
	KStar::Vec2 getVecFood(KStar::Vec2 head);
	KStar::Vec2 getVecSnakeHead();

	void addCollision();


public:

	Bobby(Univers &);
	static void clearPriority();
	void buildIA();
	void sendDirection() ;
	void calculateDirection();
	ClientTCP *getClientTCP_();
	uint16_t getId() const;

};
