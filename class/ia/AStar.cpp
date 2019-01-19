#include "AStar.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
using namespace std::placeholders;


void AStar::Generator::print() {
	return;
	for (int y = 0; y < worldSize.y; ++y) {
		for (int x = 0; x < worldSize.x; ++x) {
			if (std::find_if(walls.begin(), walls.end(), [this, y, x](Vec2i vec2i){
				return vec2i.x == x && vec2i.y == y;
			}) != walls.end())
				std::cout << std::setw(6) << "____";
			else
				std::cout << std::setw(6) << "WALL";
		}
		std::cout << std::endl;
	}
}


std::ostream &AStar::operator<<(std::ostream &os, const AStar::Vec2i &i) {
	os << "x: " << i.x << " y: " << i.y;
	return os;
}

bool AStar::Vec2i::operator==(const AStar::Vec2i &rhs) const {
	return x == rhs.x &&
		   y == rhs.y;
}

bool AStar::Vec2i::operator!=(const AStar::Vec2i &rhs) const {
	return !(rhs == *this);
}

bool AStar::Vec2i::operator<(const AStar::Vec2i &rhs) const {
	if (x < rhs.x)
		return true;
	if (rhs.x < x)
		return false;
	return y < rhs.y;
}

bool AStar::Vec2i::operator>(const AStar::Vec2i &rhs) const {
	return rhs < *this;
}

bool AStar::Vec2i::operator<=(const AStar::Vec2i &rhs) const {
	return !(rhs < *this);
}

bool AStar::Vec2i::operator>=(const AStar::Vec2i &rhs) const {
	return !(*this < rhs);
}

AStar::Vec2i operator+(const AStar::Vec2i &left_, const AStar::Vec2i &right_) {
	return {left_.x + right_.x, left_.y + right_.y};
}

AStar::Node::Node(Vec2i coordinates_, Node *parent_) {
	parent = parent_;
	coordinates = coordinates_;
	G = H = 0;
}

AStar::uint AStar::Node::getScore() {
	return G + H;
}

AStar::Generator::Generator() {
	setDiagonalMovement(false);
	setHeuristic(&Heuristic::manhattan);
	direction = {
			{0,  1},
			{1,  0},
			{0,  -1},
			{-1, 0},
			{-1, -1},
			{1,  1},
			{-1, 1},
			{1,  -1}
	};
}

void AStar::Generator::setWorldSize(Vec2i worldSize_) {
	worldSize = worldSize_;
}

void AStar::Generator::setDiagonalMovement(bool enable_) {
	directions = (enable_ ? 8 : 4);
}

void AStar::Generator::setHeuristic(HeuristicFunction heuristic_) {
	heuristic = std::bind(heuristic_, _1, _2);
}

void AStar::Generator::addCollision(Vec2i coordinates_) {
	walls.push_back(coordinates_);
}

void AStar::Generator::removeCollision(Vec2i coordinates_) {
	auto it = std::find(walls.begin(), walls.end(), coordinates_);
	if (it != walls.end()) {
		walls.erase(it);
	}
}

void AStar::Generator::clearCollisions() {
	walls.clear();
}

AStar::CoordinateList AStar::Generator::findPath(Vec2i source_, Vec2i target_) {
	Node *current = nullptr;
	NodeSet openSet, closedSet;
	openSet.insert(new Node(source_));

	while (!openSet.empty()) {
		current = *openSet.begin();
		for (auto node : openSet) {
			if (node->getScore() <= current->getScore()) {
				current = node;
			}
		}

		if (current->coordinates == target_) {
			break;
		}

		closedSet.insert(current);
		openSet.erase(std::find(openSet.begin(), openSet.end(), current));

		for (uint i = 0; i < directions; ++i) {
			Vec2i newCoordinates(current->coordinates + direction[i]);
			if (detectCollision(newCoordinates) ||
				findNodeOnList(closedSet, newCoordinates)) {
				continue;
			}

			uint totalCost = current->G + ((i < 4) ? 10 : 14);

			Node *successor = findNodeOnList(openSet, newCoordinates);
			if (successor == nullptr) {
				successor = new Node(newCoordinates, current);
				successor->G = totalCost;
				successor->H = heuristic(successor->coordinates, target_);
				openSet.insert(successor);
			} else if (totalCost < successor->G) {
				successor->parent = current;
				successor->G = totalCost;
			}
		}
	}

	CoordinateList path;
	while (current != nullptr) {
		path.push_back(current->coordinates);
		current = current->parent;
	}

	releaseNodes(openSet);
	releaseNodes(closedSet);

	return path;
}

AStar::Node *
AStar::Generator::findNodeOnList(NodeSet &nodes_, Vec2i coordinates_) {
	for (auto node : nodes_) {
		if (node->coordinates == coordinates_) {
			return node;
		}
	}
	return nullptr;
}

void AStar::Generator::releaseNodes(NodeSet &nodes_) {
	for (auto it = nodes_.begin(); it != nodes_.end();) {
		delete *it;
		it = nodes_.erase(it);
	}
}

bool AStar::Generator::detectCollision(Vec2i coordinates_) {
	return coordinates_.x < 0 || coordinates_.x >= worldSize.x ||
		coordinates_.y < 0 || coordinates_.y >= worldSize.y ||
		std::find(walls.begin(), walls.end(), coordinates_) != walls.end();
}

AStar::Vec2i AStar::Heuristic::getDelta(Vec2i source_, Vec2i target_) {
	return {abs(source_.x - target_.x), abs(source_.y - target_.y)};
}

AStar::uint AStar::Heuristic::manhattan(Vec2i source_, Vec2i target_) {
	auto delta = std::move(getDelta(source_, target_));
	return static_cast<uint>(10 * (delta.x + delta.y));
}

AStar::uint AStar::Heuristic::euclidean(Vec2i source_, Vec2i target_) {
	auto delta = std::move(getDelta(source_, target_));
	return static_cast<uint>(10 * std::sqrt(std::pow(delta.x, 2) + std::pow(delta.y, 2)));
}

AStar::uint AStar::Heuristic::octagonal(Vec2i source_, Vec2i target_) {
	auto delta = std::move(getDelta(source_, target_));
	return 10 * (delta.x + delta.y) + (-6) * std::min(delta.x, delta.y);
}