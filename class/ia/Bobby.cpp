#include "Bobby.hpp"
#include <KINU/World.hpp>
#include <network/ClientTCP.hpp>

Bobby::Bobby(Univers &univers) : univers_(univers), direction(NORTH),
								 mapSize(univers.getMapSize()) {
	generator.setDiagonalMovement(false);
	generator.setHeuristic(AStar::Heuristic::manhattan);
	baseIndex = (univers.isBorderless() ? 3 : 1);
	unsigned int size = (univers.isBorderless() ? univers.getMapSize() * 3
												: univers.getMapSize());
	generator.setWorldSize(AStar::Vec2i(size, size));
}


void Bobby::findDirection(AStar::Vec2i vecSource, AStar::CoordinateList list) {


	int index = (list.size() > 2 ? list.size() - 2 : 0);
	log_info("Direction > x : %d - y : %d", list[index].x, list[index].y);

	if (list[index].x == vecSource.x - 1 &&
		list[index].y == vecSource.y) {
		log_success("eDirection::WEST");
		direction =  eDirection::WEST;
	}
	else if (list[index].x == vecSource.x &&
		list[index].y == vecSource.y - 1) {
		log_success("eDirection::NORTH");
		direction =  eDirection::NORTH;
	}
	else if (list[index].x == vecSource.x &&
		list[index].y == vecSource.y + 1) {
		log_success("eDirection::SOUTH");
		direction = eDirection::SOUTH;
	}
	else if (list[index].x == vecSource.x + 1 &&
		list[index].y == vecSource.y) {
		log_success("eDirection::EAST");
		direction =  eDirection::EAST;
	} else {
		throw std::exception();
	}
}

void Bobby::calculateDirection() {

	mutex.lock();
	if (univers_.getWorld_().getEntitiesManager().hasEntityByTagId(
			univers_.getClientTCP_().getId() + eTag::HEAD_TAG)) {

		/** Get && add head **/

		auto vecSnake = getVecSnakeHead();

		generator.clearCollisions();

		addCollision();

		generator.removeCollision(vecSnake);

		if (univers_.getWorld_().getEntitiesManager()
		.hasEntitiesGroupId(eTag::FOOD_TAG)) {

			auto vecFood = getVecFood(vecSnake);

			generator.removeCollision(vecFood);

			AStar::CoordinateList coordinateList = generator.findPath(vecSnake, vecFood);
			try {
				if (coordinateList.size() > 1)
					findDirection(vecSnake, coordinateList);
				mutex.unlock();
				std::cout << "Find path to food" << std::endl;
				return;
			} catch (std::exception const &e) {
				std::cout << "ALERT FOOD" << std::endl;
			}

		}
		if (univers_.getWorld_().getEntitiesManager().hasEntityByTagId(
				univers_.getClientTCP_().getId() + eTag::TAIL_TAG)) {

			auto vectail = getVecSnakeTail();

			generator.removeCollision(vectail);

			try {
				findDirection(vecSnake, generator.findPath(vecSnake, vectail));
				std::cout << "Find path to tail" << std::endl;
				mutex.unlock();
				return;
			} catch (std::exception const &e) {
				std::cout << "ALERT TAIL" << std::endl;

			}
		}

	}
	std::cout << "Find path to nothing" << std::endl;
	mutex.unlock();
}

eDirection Bobby::getDirection() {
	mutex.lock();
	eDirection value = direction;
	mutex.unlock();
	return value;
}

AStar::Vec2i Bobby::getVecSnakeHead() {
	auto snakeHead = univers_.getWorld_().getEntitiesManager().getEntityByTagId(
			univers_.getClientTCP_().getId() + eTag::HEAD_TAG);

	AStar::Vec2i vecSnake;

	if (snakeHead.hasComponent<PositionComponent>()) {
		auto position = snakeHead.getComponent<PositionComponent>();
		vecSnake = AStar::Vec2i(
				position.x + (univers_.isBorderless() ? mapSize : 0),
				position.y + (univers_.isBorderless() ? mapSize : 0)
		);
		log_info("vecSnake : x %d, y: %d", vecSnake.x, vecSnake.y);
	}
	return vecSnake;
}

AStar::Vec2i Bobby::getVecSnakeTail() {
	AStar::Vec2i vecTail;

	auto snakeTail = univers_.getWorld_().getEntitiesManager().getEntityByTagId(
			univers_.getClientTCP_().getId() + eTag::TAIL_TAG);

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

	auto foods = univers_.getWorld_().getEntitiesManager()
			.getEntitiesByGroupId(eTag::FOOD_TAG);

	PositionComponent positionComponent;
	double base_sqrt = -1;

	for (auto food : foods) {
		if (food.hasComponent<PositionComponent>()) {

			auto positionFood = food.getComponent<PositionComponent>();

			for (int base_y = 0; base_y < baseIndex; ++base_y) {
				for (int base_x = 0; base_x < baseIndex; ++base_x) {

					int sqrt_x = (positionFood.x + base_x * mapSize) - head.x;
					int sqrt_y = (positionFood.y + base_y * mapSize) - head.y;
					log_trace("(%d + %d * %d) - (%d + (%d ? %d: 0))", positionFood.x ,  base_x , mapSize , head.x , univers_.isBorderless()  ,  mapSize);
					log_trace("(%d + %d * %d) - (%d + (%d ? %d: 0))", positionFood.y ,  base_y , mapSize , head.y , univers_.isBorderless()  ,  mapSize);
					double compare = std::sqrt(
							sqrt_x * sqrt_x +
							sqrt_y * sqrt_y);

					log_warn("%d = %d - %d",sqrt_x, (positionFood.x + base_x * mapSize), (head.x + (univers_.isBorderless() ? mapSize : 0)));
					log_warn("%d = %d - %d",sqrt_y, (positionFood.y + base_y * mapSize), (head.y + (univers_.isBorderless() ? mapSize : 0)));
					log_warn("%f = std::sqrt(%d * %d + %d * %d)", compare, sqrt_x,sqrt_x,sqrt_y,sqrt_y);
					if (base_sqrt == -1 || compare < base_sqrt) {
						base_sqrt = compare;
						log_success("{mapSize : %d} Position saved { %d , %d }",mapSize,  positionFood.x + base_x * mapSize, positionFood.y + base_y * mapSize);
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
	for (int base_y = 0; base_y < baseIndex; ++base_y) {
		for (int base_x = 0; base_x < baseIndex; ++base_x) {
			for (int index_y = 0; index_y < mapSize; ++index_y) {
				for (int index_x = 0; index_x < mapSize; ++index_x) {
					if (univers_.getWorld_().grid(index_x, index_y) != -1) {
						generator.addCollision(
								AStar::Vec2i(
										index_x + base_x * mapSize,
										index_y + base_y * mapSize));
//						std::cout << "Collision at : x : " << index_x + base_x * mapSize << " - y " << index_y + base_y * mapSize << std::endl;
					}
				}
			}
		}
	}
	std::cout << std::endl <<std::endl;
}





































