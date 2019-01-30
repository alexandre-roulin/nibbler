#include "Bobby.hpp"
#include <Univers.hpp>
#include <logger.h>
#include <KINU/World.hpp>
#include <KINU/Entity.hpp>

std::mutex Bobby::mutex;

std::unordered_map<KINU::Entity::ID, Bobby::ePriority> Bobby::mapPriority = {
		{0, UNDEFINED},
		{1, UNDEFINED},
		{2, UNDEFINED},
		{3, UNDEFINED},
		{4, UNDEFINED},
		{5, UNDEFINED},
		{6, UNDEFINED},
		{7, UNDEFINED},
};


Bobby::Bobby(Univers &univers)
		: univers_(univers),
		  direction(kNorth),
		  mapSize(0),
		  clientTCP_(std::make_unique<ClientTCP>(univers, true)) {

}

void Bobby::buildIA() {
	mapSize = univers_.getMapSize();
	kStar.setDiagonalMovement(false);
	kStar.setHeuristic(KStar::Heuristic::euclidean);
	baseIndex = (univers_.isBorderless() ? 3 : 1);
	unsigned int size = (univers_.isBorderless() ?
						 univers_.getMapSize() * 3 : univers_.getMapSize());
	kStar.setWorldSize(size);
}


void Bobby::sendDirection() {
	ClientTCP::InputInfo inputInfo;

	inputInfo.id = clientTCP_->getId();
	inputInfo.dir = direction;

	if (univers_.getWorld_().getEntitiesManager().hasEntityByTagId(eTag::HEAD_TAG + inputInfo.id))
		clientTCP_->write_socket(ClientTCP::add_prefix(eHeader::INPUT, &inputInfo));

}


bool Bobby::define_priority(int x, int y) {
	std::vector<KINU::Entity> entitiesHead;
	for (int id = 0; id < MAX_SNAKE; ++id) {
		if (univers_.getWorld_().getEntitiesManager().hasEntityByTagId(id + eTag::HEAD_TAG))
			entitiesHead.push_back(univers_.getWorld_().getEntitiesManager().getEntityByTagId(id + eTag::HEAD_TAG));
	}
	std::vector<KINU::Entity> entitiesFiltered(entitiesHead.size());
	auto it = std::copy_if(entitiesHead.begin(), entitiesHead.end(), entitiesFiltered.begin(),
			[x, y](KINU::Entity entity){
		if (!entity.hasComponent<PositionComponent>()) return false;
		PositionComponent positionComponent = entity.getComponent<PositionComponent>();
		return std::sqrt(std::pow(positionComponent.x - x, 2) + std::pow(positionComponent.y - y, 2)) == 1 && entity.hasGroupId();
	});
	entitiesFiltered.resize(std::distance(entitiesFiltered.begin(),it));
//	log_warn(" %d define_priority::entitiesFiltered %d", getId(), entitiesFiltered.size());

	if (entitiesFiltered.size() < 2 || mapPriority[getId()] == PRIORITY) return false;

	if (std::any_of(entitiesFiltered.begin(), entitiesFiltered.end(), [](KINU::Entity e){ return mapPriority[e.getId()] == PRIORITY;})) {
		mapPriority[getId()] = NO_PRIORITY;
		return true;
	}

	mapPriority[getId()] = NO_PRIORITY;
	int ran = rand() % entitiesFiltered.size();
	mapPriority[entitiesFiltered[ran].getGroupIdByEntity()] = PRIORITY;
//	log_success("%d mapPriority[%d] == %d", getId(), entitiesFiltered[ran].getGroupIdByEntity(), mapPriority[entitiesFiltered[ran].getGroupIdByEntity()]);
	return mapPriority[getId()] == NO_PRIORITY;
}

void Bobby::findDirection(KStar::Vec2 vecSource, KStar::Vec2 vecTarget) {


//	if (getId() == 0)
//		std::cout << "VecSource : " << vecSource << " Direction : "<< vecTarget << std::endl;
	mutex.lock();

//	log_warn("B4 mapPriority[%d] %d",getId(), mapPriority[getId()]);
	if (mapPriority[getId()] == UNDEFINED)
		if (define_priority(vecTarget.x, vecTarget.y)) {
//			std::for_each(mapPriority.begin(), mapPriority.end(), [](std::pair<KINU::Entity::ID, Bobby::ePriority> pair){ std::cout << pair.first << " " << pair.second << std::endl; });
			mutex.unlock();
			throw std::exception();
		}
	mutex.unlock();
//	log_warn("A6 mapPriority[%d] %d",getId(), mapPriority[getId()]);

	if (vecTarget.x == vecSource.x - 1 &&
		vecTarget.y == vecSource.y) {
//		log_success("eDirection::WEST");
		direction = eDirection::kWest;
	} else if (vecTarget.x == vecSource.x &&
			   vecTarget.y == vecSource.y - 1) {
//		log_success("eDirection::North");
		direction = eDirection::kNorth;
	} else if (vecTarget.x == vecSource.x &&
			   vecTarget.y == vecSource.y + 1) {
//		log_success("eDirection::SOUTH");
		direction = eDirection::kSouth;
	} else if (vecTarget.x == vecSource.x + 1 &&
			   vecTarget.y == vecSource.y) {
//		log_success("eDirection::EAST");
		direction = eDirection::kEast;
	} else {
		throw std::exception();

	}
}

void Bobby::calculateDirection() {


	if (univers_.getWorld_().getEntitiesManager().hasEntityByTagId(
			clientTCP_->getId() + eTag::HEAD_TAG)) {

		kStar.clearCollisions();
		addCollision();
		/** Get && add head **/

		auto vecSnake = getVecSnakeHead();

		kStar.removeCollision(vecSnake);

		if (univers_.getWorld_().getEntitiesManager()
				.hasEntitiesGroupId(eTag::FOOD_TAG) || univers_.getWorld_().getEntitiesManager()
																		   .hasEntitiesGroupId(eTag::FOOD_TAG_FROM_SNAKE)) {

			auto vecFood = getVecFood(vecSnake);

			kStar.removeCollision(vecFood);

			KStar::Path path = kStar.searchPath(vecSnake, vecFood);

			if (path.size() > 1) {
				try {
					findDirection(vecSnake, path[1]);
					sendDirection();
					log_warn("FOOD::Pathfinding[%d] path.size()[%d]", getId(), path.size());
					return;
				} catch (std::exception const &e) {
					log_error("WARN::NO_FOOD");
				}
			}
			kStar.addCollision(vecFood);
		}
		if (univers_.getWorld_().getEntitiesManager().hasEntityByTagId(
				clientTCP_->getId() + eTag::TAIL_TAG)) {

			auto vecTail = getVecSnakeTail();

			kStar.removeCollision(vecTail);

				KStar::Path path = kStar.searchPath(vecSnake, vecTail);
				if (path.size() > 1) {
					try {
						findDirection(vecSnake, path[1]);
						sendDirection();
						log_warn("TAIL::Pathfinding[%d] path.size()[%d]", getId(), path.size());
						return;
					} catch (std::exception const &e) {
						log_error("WARN::NO_TAIL");
					}
				}
		}

	}
	sendDirection();
}

KStar::Vec2 Bobby::getVecSnakeHead() {
	auto snakeHead = univers_.getWorld_().getEntitiesManager().getEntityByTagId(
			clientTCP_->getId() + eTag::HEAD_TAG);

	KStar::Vec2 vecSnake;

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
	KStar::Vec2 vecTail;

	auto snakeTail = univers_.getWorld_().getEntitiesManager().getEntityByTagId(
			clientTCP_->getId() + eTag::TAIL_TAG);

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
	std::vector<KINU::Entity> foods;
	if (univers_.getWorld_().getEntitiesManager().hasEntitiesGroupId(eTag::FOOD_TAG)) {
		auto food_tag = univers_.getWorld_().getEntitiesManager().getEntitiesByGroupId(
				eTag::FOOD_TAG);
		foods.insert(foods.end(), food_tag.begin(), food_tag.end());
	}

	if (univers_.getWorld_().getEntitiesManager().hasEntitiesGroupId(eTag::FOOD_TAG_FROM_SNAKE)) {
		auto food_tag = univers_.getWorld_().getEntitiesManager().getEntitiesByGroupId(
				eTag::FOOD_TAG_FROM_SNAKE);
		foods.insert(foods.end(), food_tag.begin(), food_tag.end());
	}


	for (auto food : foods)	 {
		if (food.hasComponent<PositionComponent>()) {

			auto positionFood = food.getComponent<PositionComponent>();
//			log_warn("Food : %d - Position : %d",eSprite::FOOD, grid_(positionComponent.x, positionComponent.y));
//			if (grid_(positionComponent.x, positionComponent.y) == eSprite::FOOD)
			for (int base_y = 0; base_y < baseIndex; ++base_y) {
				for (int base_x = 0; base_x < baseIndex; ++base_x) {
					int scale_x = positionFood.x + base_x * mapSize;
					int scale_y = positionFood.y + base_y * mapSize;
//				log_trace("(%d + %d * %d) - (%d + (%d ? %d: 0))",
//						  positionFood.x, base_x, mapSize, head.x,
//						  univers_.isBorderless(), mapSize);
//				log_trace("(%d + %d * %d) - (%d + (%d ? %d: 0))",
//						  positionFood.y, base_y, mapSize, head.y,
//						  univers_.isBorderless(), mapSize);
					compare = std::sqrt(
							std::pow(scale_x - head.x, 2) +
							std::pow(scale_y - head.y, 2)
					);
//				log_warn("%d = %d - %d", sqrt_x,
//						 (positionFood.x + base_x * mapSize), (head.x +
//															   (univers_.isBorderless()
//																? mapSize
//																: 0)));
//				log_warn("%d = %d - %d", sqrt_y,
//						 (positionFood.y + base_y * mapSize), (head.y +
//															   (univers_.isBorderless()
//																? mapSize
//																: 0)));
					int x = positionFood.x + base_x * mapSize;
					int y = positionFood.y + base_y * mapSize;
					//&&
					//					std::count_if(vecPosition.begin(), vecPosition.end(), [x, y](PositionComponent const &p){
					//						return p.x == x && p.y == y;
					//					}) == 1)

					if (base_sqrt == -1 || (compare < base_sqrt && (univers_.getWorld_().grid(x, y) & eSprite::FOOD) == eSprite::FOOD)) {
						base_sqrt = compare;
//					log_success("{mapSize : %d} Position saved { %d , %d }",
//								mapSize, positionFood.x + base_x * mapSize,
//								positionFood.y + base_y * mapSize);
						positionComponent = PositionComponent(
								scale_x,
								scale_y
						);
					}
				}
			}
		}
	}
//	log_info("vecFood : x %d, y: %d", positionComponent.x, positionComponent.y);
	return KStar::Vec2(positionComponent.x, positionComponent.y);
}

void Bobby::addCollision() {
	auto entities = univers_.getWorld_().getEntitiesManager().getEntities();
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
	return clientTCP_->getId();
}

ClientTCP *Bobby::getClientTCP_() {
	return clientTCP_.get();
}

void Bobby::clearPriority() {
	for (int id = 0; id < MAX_SNAKE; ++id) {
		mapPriority[id] = UNDEFINED;
	}
}


































