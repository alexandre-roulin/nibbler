#include "Bobby.hpp"
#include <KINU/World.hpp>
#include <network/ClientTCP.hpp>

Bobby::Bobby(Univers &univers) : univers_(univers), direction(NORTH) {
	generator.setDiagonalMovement(false);
	generator.setHeuristic(AStar::Heuristic::manhattan);
	generator.setWorldSize(AStar::Vec2i(univers.getMapSize(),
										univers.getMapSize())); // TODO BORDERLESS
}


void Bobby::findDirection(AStar::Vec2i vecSource, AStar::CoordinateList list) {
//	for (auto item : list) {
//		std::cout << item << std::endl;
//	}

	int index = (list.size() > 2 ? list.size() - 2 : 0);
//	log_debug("Snake { x:%d , y:%d }",vecSource.x, vecSource.y);
//	log_debug("Target { x:%d , y:%d }",list[index].x, list[index].y);
//	log_warn("Index : %d Size : %d", index, list.size());

//	std::cout << list[index] << std::endl;
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
	}else {

		throw std::exception();
	}
//	log_fatal("Bobby::findDirection.don't find x %d y %d size %d", vecSource.x, vecSource.y, list.size());
}

void Bobby::calculateDirection() {

	mutex.lock();
	if (univers_.getWorld_().getEntitiesManager().hasEntityByTagId(
			univers_.getClientTCP_().getId() + eTag::HEAD_TAG)) {
		std::vector<AStar::CoordinateList> coordinateListFood;

		auto snakeHead = univers_.getWorld_().getEntitiesManager().getEntityByTagId(
				univers_.getClientTCP_().getId() + eTag::HEAD_TAG);

		AStar::Vec2i vecSnake;

		if (snakeHead.hasComponent<PositionComponent>()) {
			auto position = snakeHead.getComponent<PositionComponent>();
			vecSnake = AStar::Vec2i(position.x, position.y);
		}
		generator.clearCollisions();
		for (int index_y = 0; index_y < univers_.getMapSize(); ++index_y) {
			for (int index_x = 0; index_x < univers_.getMapSize(); ++index_x) {
				if (univers_.getWorld_().grid(index_x, index_y) != -1)
					generator.addCollision(AStar::Vec2i(index_x, index_y));
			}
		}
		generator.removeCollision(vecSnake);
		if (univers_.getWorld_().getEntitiesManager().hasEntitiesGroupId(
				eTag::FOOD_TAG)) {

			auto foods = univers_.getWorld_().getEntitiesManager().getEntitiesByGroupId(
					eTag::FOOD_TAG);
			for (auto food : foods) {
				if (!food.hasComponent<PositionComponent>())
					continue;
				auto positionFood = food.getComponent<PositionComponent>();
				AStar::Vec2i vecFood(positionFood.x, positionFood.y);
				generator.removeCollision(vecFood);
				coordinateListFood.push_back(
						generator.findPath(vecSnake, vecFood));
			}
			std::sort(coordinateListFood.begin(), coordinateListFood.end());
			if (!coordinateListFood.empty() && coordinateListFood[0].size() > 1) {
				try {
					log_info("Bobby::findDirection food");

					findDirection(vecSnake, coordinateListFood[0]);
					mutex.unlock();
					return;
				} catch (std::exception const &e) {
					log_fatal("catch food");
				}
			}
		}
		if (univers_.getWorld_().getEntitiesManager().hasEntityByTagId(
				univers_.getClientTCP_().getId() + eTag::TAIL_TAG)) {
			AStar::Vec2i vecTail;

			auto snakeTail = univers_.getWorld_().getEntitiesManager().getEntityByTagId(
					univers_.getClientTCP_().getId() + eTag::TAIL_TAG);

			if (snakeTail.hasComponent<PositionComponent>()) {
				auto position = snakeTail.getComponent<PositionComponent>();
				vecTail = AStar::Vec2i(position.x, position.y);
			}
			try {
				log_info("Bobby::findDirection tail");
				findDirection(vecSnake, generator.findPath(vecSnake, vecTail));
				mutex.unlock();
				return;
			} catch (std::exception const &e){

				log_fatal("catch tail");
			}
		}

	}
	mutex.unlock();
}

eDirection Bobby::getDirection() {
	mutex.lock();
	eDirection value = direction;
	log_info("Direction %d", direction);
	mutex.unlock();
	return value;
}





































