#pragma once

#include <nibbler.hpp>
#include <map>
#include <network/SnakeClient.hpp>
#include <KINU/Entity.hpp>
#include "KStar.hpp"


class Univers;

class Bobby {
public:

	Bobby(Univers &);
	static void clearPriority();
	void buildIA();
	void sendDirection() ;
	void calculateDirection();
	SnakeClient *getClientTCP_();
	uint16_t getId() const;

private:

	enum ePriority {
		kUndefined,		//0
		kNoPriority,	//1
		kPriority		//2
	};

	static std::unordered_map<KINU::Entity::ID, ePriority> mapPriority;

	Univers &univers_;
	eDirection direction;
	unsigned int mapSize;
	unsigned int baseIndex;
	static std::mutex mutex;
	KStar kStar;
	boost::shared_ptr<SnakeClient> clientTCP_;
	void findDirection(KStar::Vec2 vecSource, KStar::Vec2 vecTarget);
	bool define_priority(int x, int y);
	KStar::Vec2 getVecSnakeTail();
	KStar::Vec2 getVecFood(KStar::Vec2 head);
	KStar::Vec2 getVecSnakeHead();
	void addCollision();


};
