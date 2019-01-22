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
		  direction(NORTH),
		  mapSize(0),
		  clientTCP_(std::make_unique<ClientTCP>(univers, true)) {

}

void Bobby::buildIA() {
	mapSize = univers_.getMapSize();
	generator.setDiagonalMovement(false);
	generator.setHeuristic(AStar::Heuristic::manhattan);
	baseIndex = (univers_.isBorderless() ? 3 : 1);
	unsigned int size = (univers_.isBorderless() ?
						 univers_.getMapSize() * 3 : univers_.getMapSize());
	generator.setWorldSize(AStar::Vec2i(size, size));
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

void Bobby::findDirection(AStar::Vec2i vecSource, AStar::CoordinateList list) {

	int index = (list.size() > 2 ? list.size() - 2 : 0);

	if (getId() == 0)
		std::cout << "VecSource : " << vecSource << " Direction : "<< list[index] << std::endl;
	auto pool = univers_.getWorld_().getEntitiesManager().getComponents<PositionComponent>();

	mutex.lock();

//	log_warn("B4 mapPriority[%d] %d",getId(), mapPriority[getId()]);
	if (mapPriority[getId()] == UNDEFINED)
		if (define_priority(list[index].x, list[index].y)) {
			mutex.unlock();
			throw std::exception();
		}
	mutex.unlock();
//	log_warn("A6 mapPriority[%d] %d",getId(), mapPriority[getId()]);

	if (list[index].x == vecSource.x - 1 &&
		list[index].y == vecSource.y) {
//		log_success("eDirection::WEST");
		direction = eDirection::WEST;
	} else if (list[index].x == vecSource.x &&
			   list[index].y == vecSource.y - 1) {
//		log_success("eDirection::NORTH");
		direction = eDirection::NORTH;
	} else if (list[index].x == vecSource.x &&
			   list[index].y == vecSource.y + 1) {
//		log_success("eDirection::SOUTH");
		direction = eDirection::SOUTH;
	} else if (list[index].x == vecSource.x + 1 &&
			   list[index].y == vecSource.y) {
//		log_success("eDirection::EAST");
		direction = eDirection::EAST;
	} else {
		log_success("eDirection::THROW");
		throw std::exception();
	}
}

void Bobby::calculateDirection() {
	if (univers_.getWorld_().getEntitiesManager().hasEntityByTagId(
			clientTCP_->getId() + eTag::HEAD_TAG)) {

		/** Get && add head **/

		auto vecSnake = getVecSnakeHead();

		generator.clearCollisions();

		addCollision();

		generator.removeCollision(vecSnake);

		if (univers_.getWorld_().getEntitiesManager()
				.hasEntitiesGroupId(eTag::FOOD_TAG) || univers_.getWorld_().getEntitiesManager()
																		   .hasEntitiesGroupId(eTag::FOOD_TAG_FROM_SNAKE)) {

			auto vecFood = getVecFood(vecSnake);

			generator.removeCollision(vecFood);


			AStar::CoordinateList coordinateList = generator.findPath(vecSnake,
																	  vecFood);
			try {
				if (coordinateList.size() > 1)
					findDirection(vecSnake, coordinateList);
				log_warn("FOOD::Pathfinding[%d]", getId());
				sendDirection();
				return;
			} catch (std::exception const &e) {
				std::cout << "ALERT FOOD" << std::endl;
			}

		}
		if (univers_.getWorld_().getEntitiesManager().hasEntityByTagId(
				clientTCP_->getId() + eTag::TAIL_TAG)) {

			auto vecTail = getVecSnakeTail();

			generator.removeCollision(vecTail);

			try {
				AStar::CoordinateList coordinateList = generator.findPath(vecSnake, vecTail);
				findDirection(vecSnake, coordinateList);
				log_warn("TAIL::Pathfinding[%d]", getId());
				sendDirection();
				return;
			} catch (std::exception const &e) {
				std::cout << "ALERT TAIL" << std::endl;

			}
		}

		try {
			log_warn("ANY::Pathfinding[%d]", getId());
			findAnyDirectionValid(vecSnake);
		} catch (std::exception const &e) {
			std::cout << "Find path to nothing" << std::endl;
		}
	}

	sendDirection();
}

AStar::Vec2i Bobby::getVecSnakeHead() {
	auto snakeHead = univers_.getWorld_().getEntitiesManager().getEntityByTagId(
			clientTCP_->getId() + eTag::HEAD_TAG);

	AStar::Vec2i vecSnake;

	if (snakeHead.hasComponent<PositionComponent>()) {
		auto position = snakeHead.getComponent<PositionComponent>();
		vecSnake = AStar::Vec2i(
				position.x + (univers_.isBorderless() ? mapSize : 0),
				position.y + (univers_.isBorderless() ? mapSize : 0)
		);
//		log_info("vecSnake : x %d, y: %d", vecSnake.x, vecSnake.y);
	}
	return vecSnake;
}

AStar::Vec2i Bobby::getVecSnakeTail() {
	AStar::Vec2i vecTail;

	auto snakeTail = univers_.getWorld_().getEntitiesManager().getEntityByTagId(
			clientTCP_->getId() + eTag::TAIL_TAG);

	if (snakeTail.hasComponent<PositionComponent>()) {
		auto position = snakeTail.getComponent<PositionComponent>();
		vecTail = AStar::Vec2i(
				position.x + (univers_.isBorderless() ? mapSize : 0),
				position.y + (univers_.isBorderless() ? mapSize : 0)
		);
	}
	return vecTail;
}

AStar::Vec2i Bobby::getVecFood(AStar::Vec2i head) {

	PositionComponent positionComponent;
	double base_sqrt = -1;
	double compare;
	std::vector<KINU::Entity> foods;

	if (univers_.getWorld_().getEntitiesManager().hasEntitiesGroupId(eTag::FOOD_TAG)) {
		auto food_tag = univers_.getWorld_().getEntitiesManager().getEntitiesByGroupId(
				eTag::FOOD_TAG);
		log_warn("Sizet [%d]", food_tag.size());
		foods.insert(foods.end(), food_tag.begin(), food_tag.end());
	}

	if (univers_.getWorld_().getEntitiesManager().hasEntitiesGroupId(eTag::FOOD_TAG_FROM_SNAKE)) {
		auto food_tag = univers_.getWorld_().getEntitiesManager().getEntitiesByGroupId(
				eTag::FOOD_TAG_FROM_SNAKE);
		log_warn("Sizets [%d]", food_tag.size());
		foods.insert(foods.end(), food_tag.begin(), food_tag.end());
	}

	log_warn("Size [%d]", foods.size());

	for (auto food : foods)	 {
		log_warn("food.hasComponent<PositionComponent>() %s", food.hasComponent<PositionComponent>() ? "True" : "False" );
		if (food.hasComponent<PositionComponent>()) {

			auto positionFood = food.getComponent<PositionComponent>();
			generator.removeCollision(AStar::Vec2i(positionFood.x, positionFood.y));
//			log_warn("Food : %d - Position : %d",eSprite::FOOD, grid_(positionComponent.x, positionComponent.y));
//			if (grid_(positionComponent.x, positionComponent.y) == eSprite::FOOD)
			for (int base_y = 0; base_y < baseIndex; ++base_y) {
				for (int base_x = 0; base_x < baseIndex; ++base_x) {
					int sqrt_x = (positionFood.x + base_x * mapSize) - head.x;
					int sqrt_y = (positionFood.y + base_y * mapSize) - head.y;
//				log_trace("(%d + %d * %d) - (%d + (%d ? %d: 0))",
//						  positionFood.x, base_x, mapSize, head.x,
//						  univers_.isBorderless(), mapSize);
//				log_trace("(%d + %d * %d) - (%d + (%d ? %d: 0))",
//						  positionFood.y, base_y, mapSize, head.y,
//						  univers_.isBorderless(), mapSize);
					compare = std::sqrt(
							std::pow(sqrt_x, 2) +
							std::pow(sqrt_y, 2)
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
				log_warn("comapre : %f base_sqrt : %f", compare, base_sqrt);
					if (base_sqrt == -1 || compare < base_sqrt) {
						base_sqrt = compare;
//					log_success("{mapSize : %d} Position saved { %d , %d }",
//								mapSize, positionFood.x + base_x * mapSize,
//								positionFood.y + base_y * mapSize);
						positionComponent = PositionComponent(
								positionFood.x + base_x * mapSize,
								positionFood.y + base_y * mapSize
						);
					}
				}
			}
		}
	}
	log_info("vecFood : x %d, y: %d", positionComponent.x, positionComponent.y);
	return AStar::Vec2i(positionComponent.x, positionComponent.y);
}

void Bobby::addCollision() {
	auto pool = univers_.getWorld_().getEntitiesManager().getComponents<PositionComponent>();
	std::for_each(pool.begin(), pool.end(), [this](PositionComponent p){
		for (int base_y = 0; base_y < baseIndex; ++base_y) {
			for (int base_x = 0; base_x < baseIndex; ++base_x) {
				generator.addCollision(
						AStar::Vec2i(
								p.x + base_x * mapSize,
								p.y + base_y * mapSize
								)
						);
			}
		}
	});
//	grid.print();
}

uint16_t Bobby::getId() const {
	return clientTCP_->getId();
}

ClientTCP *Bobby::getClientTCP_() {
	return clientTCP_.get();
}

void Bobby::clearPriority() {
//	log_fatal("Bobby::clearPriority");

	for (int id = 0; id < MAX_SNAKE; ++id) {
		mapPriority[id] = UNDEFINED;
	}
}

void Bobby::findAnyDirectionValid(AStar::Vec2i vecSnake) {

	static int constexpr direction[8][2] = {
			{0,  -1},
			{0,  1},
			{-1, 0},
			{1,  0},
			{-1, -1},
			{-1, 1},
			{1,  -1},
			{1,  1}
	};

	static int const SCALE_X = 0;
	static int const SCALE_Y = 1;
	unsigned int max = 4;

	std::vector<KINU::Entity> entitiesHead;
	for (int id = 0; id < MAX_SNAKE; ++id) {
		if (univers_.getWorld_().getEntitiesManager().hasEntityByTagId(id + eTag::HEAD_TAG))
			entitiesHead.push_back(univers_.getWorld_().getEntitiesManager().getEntityByTagId(id + eTag::HEAD_TAG));
	}

	for (int base = 0; base < max; ++base) {

		int x = vecSnake.x + direction[base][SCALE_X];
		int y = vecSnake.x + direction[base][SCALE_Y];

		std::vector<KINU::Entity> entitiesFiltered(entitiesHead.size());

		auto it = std::copy_if(entitiesHead.begin(), entitiesHead.end(), entitiesFiltered.begin(),
							   [x, y](KINU::Entity entity){
								   if (!entity.hasComponent<PositionComponent>()) return false;
								   PositionComponent positionComponent = entity.getComponent<PositionComponent>();
								   return std::sqrt(std::pow(positionComponent.x - x, 2) + std::pow(positionComponent.y - y, 2)) == 1 && entity.hasGroupId();
							   });
		entitiesFiltered.resize(std::distance(entitiesFiltered.begin(),it));
		auto p = PositionComponent(x, y);
		AStar::Vec2i vec2i(p.x, p.y);
		if (entitiesFiltered.size() < 2 && !generator.detectCollision(vec2i)) {
			findDirection(vecSnake, std::vector<AStar::Vec2i>{AStar::Vec2i(x, y)});
			return;
		}
		entitiesFiltered.clear();
	}
}



































