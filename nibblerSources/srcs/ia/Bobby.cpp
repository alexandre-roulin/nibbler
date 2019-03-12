#include "Bobby.hpp"
#include <cores/Univers.hpp>
#include <KINU/World.hpp>
#include <KINU/Entity.hpp>
#include <cores/GameManager.hpp>

std::mutex Bobby::mutex;

std::unordered_map<KINU::Entity::ID, Bobby::ePriority> Bobby::mapPriority = {
		{0, kUndefined},
		{1, kUndefined},
		{2, kUndefined},
		{3, kUndefined},
		{4, kUndefined},
		{5, kUndefined},
		{6, kUndefined},
		{7, kUndefined},
};


Bobby::Bobby(Univers &univers)
		: univers_(univers),
		  direction(kNorth),
		  mapSize(0),
		  clientTCP_(SnakeClient::create(univers, true)) {
}

void Bobby::buildIA() {
	mapSize = univers_.getMapSize();
	kStar.setDiagonalMovement(false);
	kStar.setHeuristic(KStar::Heuristic::euclidean);
	baseIndex = (univers_.isBorderless() ? 3 : 1);
	unsigned int size = (univers_.isBorderless() ? mapSize * 3 : mapSize);
	kStar.setWorldSize(KStar::Vec2(size,size));
}


void Bobby::sendDirection() {

	std::shared_ptr<KINU::World> world = univers_.getGameManager().getWorld_();
	if (!world) return;

	if (world->getEntitiesManager().hasEntityByTagId(eTag::kHeadTag + clientTCP_->getId_())) {
		clientTCP_->sendDirection(direction);
	}

}


bool Bobby::define_priority(int x, int y) {
	std::vector<KINU::Entity> entitiesHead;
	std::shared_ptr<KINU::World> world = univers_.getGameManager().getWorld_();
	if (!world) return false;

	for (int id = 0; id < SNAKE_MAX; ++id) {
		if (world->getEntitiesManager().hasEntityByTagId(id + eTag::kHeadTag))
			entitiesHead.push_back(world->getEntitiesManager().getEntityByTagId(id + eTag::kHeadTag));
	}

	if (entitiesHead.size() == 1) {
		return false;
	}

	std::vector<KINU::Entity> entitiesFiltered(entitiesHead.size());

	auto it = std::copy_if(entitiesHead.begin(), entitiesHead.end(), entitiesFiltered.begin(),
			[x, y](KINU::Entity entity){
		if (!entity.hasComponent<PositionComponent>()) return false;
		PositionComponent positionComponent = entity.getComponent<PositionComponent>();
		return std::sqrt(std::pow(positionComponent.x - x, 2) + std::pow(positionComponent.y - y, 2)) == 1 && entity.hasGroupId();
	});

	entitiesFiltered.resize(std::distance(entitiesFiltered.begin(),it));

	if (entitiesFiltered.size() < 2 || mapPriority[getId()] == kPriority) return false;
	if (entitiesHead.size() == 1) {
		return false;
	}
	if (std::any_of(entitiesFiltered.begin(), entitiesFiltered.end(), [](KINU::Entity e){ return mapPriority[e.getId()] == kPriority;})) {
		return true;
	}

	int ran = rand() % entitiesFiltered.size();
	mapPriority[entitiesFiltered[ran].getGroupIdByEntity()] = kPriority;
	return mapPriority[getId()] != kPriority;
}

void Bobby::findDirection(KStar::Vec2 vecSource, KStar::Vec2 vecTarget) {


	mutex.lock();

	if (mapPriority[getId()] == kUndefined)
		if (define_priority(vecTarget.x, vecTarget.y)) {
			mutex.unlock();
			throw std::exception();
		}
	mutex.unlock();

	if (vecTarget.x == vecSource.x - 1 &&
		vecTarget.y == vecSource.y) {
		direction = eDirection::kWest;
	} else if (vecTarget.x == vecSource.x &&
			   vecTarget.y == vecSource.y - 1) {
		direction = eDirection::kNorth;
	} else if (vecTarget.x == vecSource.x &&
			   vecTarget.y == vecSource.y + 1) {
		direction = eDirection::kSouth;
	} else if (vecTarget.x == vecSource.x + 1 &&
			   vecTarget.y == vecSource.y) {
		direction = eDirection::kEast;
	} else {
		throw std::exception();
	}
}

void Bobby::calculateDirection() {
	std::shared_ptr<KINU::World> world = univers_.getGameManager().getWorld_();

	if (!world) return;
	if (world->getEntitiesManager().hasEntityByTagId(
			clientTCP_->getId_() + eTag::kHeadTag)) {

		kStar.clearCollisions();
		addCollision();
		/** Get && add head **/

		auto vecSnake = getVecSnakeHead();

		kStar.removeCollision(vecSnake);
		if (world->getEntitiesManager()
				.hasEntitiesGroupId(eTag::kFoodTag) || world->getEntitiesManager()
																		   .hasEntitiesGroupId(eTag::kFoodFromSnake)) {

			auto vecFood = getVecFood(vecSnake);

			kStar.removeCollision(vecFood);

			KStar::Path path = kStar.searchPath(vecSnake, vecFood);
			if (path.size() > 1) {
				try {
					findDirection(vecSnake, path[1]);
					sendDirection();
					return;
				} catch (std::exception const &e) {
				}
			}
			kStar.addCollision(vecFood);
		}
		if (world->getEntitiesManager().hasEntityByTagId(
				clientTCP_->getId_() + eTag::kTailTag)) {

			auto vecTail = getVecSnakeTail();

			kStar.removeCollision(vecTail);

				KStar::Path path = kStar.searchPath(vecSnake, vecTail);
				if (path.size() > 1) {
					try {
						findDirection(vecSnake, path[1]);
						sendDirection();
						return;
					} catch (std::exception const &e) {
					}
				}
		}
		for (int idxDir = 0; idxDir < kStar.getDirection(); ++idxDir) {
			auto checkVec2 = vecSnake + kStar.getDirections()[idxDir];
			if (!kStar.isCollision(checkVec2)) {
				try {
					findDirection(vecSnake, checkVec2);
					sendDirection();
					return;
				} catch (std::exception const &e) {
				}
			}
		}
	}
	sendDirection();
}

KStar::Vec2 Bobby::getVecSnakeHead() {
	KStar::Vec2 vecSnake;

	std::shared_ptr<KINU::World> world = univers_.getGameManager().getWorld_();
	if (!world)
		return vecSnake;
	auto snakeHead = world->getEntitiesManager().getEntityByTagId(
			clientTCP_->getId_() + eTag::kHeadTag);

	if (snakeHead.hasComponent<PositionComponent>()) {
		auto position = snakeHead.getComponent<PositionComponent>();
		vecSnake = KStar::Vec2(
				position.x + (univers_.isBorderless() ? mapSize : 0),
				position.y + (univers_.isBorderless() ? mapSize : 0)
		);
	}
	return vecSnake;
}

KStar::Vec2 Bobby::getVecSnakeTail() {
	std::shared_ptr<KINU::World> world = univers_.getGameManager().getWorld_();

	KStar::Vec2 vecTail;
	if (!world)
		return vecTail;
	auto snakeTail = world->getEntitiesManager().getEntityByTagId(
			clientTCP_->getId_() + eTag::kTailTag);

	if (snakeTail.hasComponent<PositionComponent>()) {
		auto position = snakeTail.getComponent<PositionComponent>();
		vecTail = KStar::Vec2(
				position.x + (univers_.isBorderless() ? mapSize : 0),
				position.y + (univers_.isBorderless() ? mapSize : 0)
		);
	}
	return vecTail;
}

KStar::Vec2 Bobby::getVecFood(KStar::Vec2 head) {

	PositionComponent positionComponent;
	double base_sqrt = -1;
	double compare;
	std::shared_ptr<KINU::World> world = univers_.getGameManager().getWorld_();
	std::vector<KINU::Entity> foods;

	if (world && world->getEntitiesManager().hasEntitiesGroupId(eTag::kFoodTag)) {
		auto food_tag = world->getEntitiesManager().getEntitiesByGroupId(
				eTag::kFoodTag);
		foods.insert(foods.end(), food_tag.begin(), food_tag.end());
	}

	if (world && world->getEntitiesManager().hasEntitiesGroupId(eTag::kFoodFromSnake)) {
		auto food_tag = world->getEntitiesManager().getEntitiesByGroupId(
				eTag::kFoodFromSnake);
		foods.insert(foods.end(), food_tag.begin(), food_tag.end());
	}


	for (auto food : foods)	 {
		if (food.hasComponent<PositionComponent>()) {

			auto positionFood = food.getComponent<PositionComponent>();
			for (size_t base_y = 0; base_y < baseIndex; ++base_y) {
				for (size_t base_x = 0; base_x < baseIndex; ++base_x) {
					int scale_x = positionFood.x + base_x * mapSize;
					int scale_y = positionFood.y + base_y * mapSize;
					compare = std::sqrt(
							std::pow(scale_x - head.x, 2) +
							std::pow(scale_y - head.y, 2)
					);
					if (base_sqrt == -1 || (compare < base_sqrt && (univers_.getGrid_()(positionFood.x, positionFood.y) & eSprite::kFood) == eSprite::kFood)) {
						base_sqrt = compare;
						positionComponent = PositionComponent(
								scale_x,
								scale_y
						);
					}
				}
			}
		}
	}
	return KStar::Vec2(positionComponent.x, positionComponent.y);
}

void Bobby::addCollision() {
	std::shared_ptr<KINU::World> world = univers_.getGameManager().getWorld_();
	if (!world) return;
	auto entities = world->getEntitiesManager().getEntities();
	for (auto &entity : entities) {
		if (entity.hasComponent<PositionComponent>()) {
			auto positionComponent = entity.getComponent<PositionComponent>();
			kStar.addCollision(KStar::Vec2(
					positionComponent.x,
					positionComponent.y
					));
		}

	}
}

uint16_t Bobby::getId() const {
	return clientTCP_->getId_();
}

SnakeClient *Bobby::getClientTCP_() {
	return clientTCP_.get();
}

void Bobby::clearPriority() {
	for (int id = 0; id < SNAKE_MAX; ++id) {
		mapPriority[id] = kUndefined;
	}
}

Bobby::~Bobby() {
	clientTCP_ = nullptr;
}


































