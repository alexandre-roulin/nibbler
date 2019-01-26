#include "KStar.hpp"
#include "Grid.tpp"
#include <cmath>
#include <cstdlib>
#include <type_traits>
#include <array>

/** Constant **/

std::array<KStar::Vec2, 8> const KStar::directions = {
		Vec2(0,  1),
		Vec2(1,  0),
		Vec2(0,  -1),
		Vec2(-1, 0),
		Vec2(-1, -1),
		Vec2(1,  1),
		Vec2(-1, 1),
		Vec2(1,  -1)
};

/** Node **/

KStar::Node::operator bool() const {
	return H != 0 && vec.x != 0 && vec.y != 0;
}

bool KStar::Node::operator==(const KStar::Node &rhs) const {
	return vec == rhs.vec;
}

bool KStar::Node::operator!=(const KStar::Node &rhs) const {
	return !(rhs == *this);
}

KStar::Node::Node(const KStar::Vec2 vec, double H, double G, double F)
:
vec(vec),H(H),
G(G),
F(F) {
	parent = nullptr;

}

/** Vec2 **/

KStar::Vec2::Vec2(int x, int y) : x(x), y(y) {

}

KStar::Vec2 KStar::Vec2::operator+(const KStar::Vec2 &lhs) {
	return KStar::Vec2(x + lhs.x, y + lhs.y);
}

KStar::Vec2::Vec2(const KStar::Vec2 &vec) {
	x = vec.x;
	y = vec.y;
}

bool KStar::Vec2::operator==(const KStar::Vec2 &rhs) const {
	return x == rhs.x &&
		   y == rhs.y;
}

bool KStar::Vec2::operator!=(const KStar::Vec2 &rhs) const {
	return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const KStar::Vec2 &vec2) {
	os << "x: " << vec2.x << " y: " << vec2.y;
	return os;
}

/** KStar **/

KStar::KStar() :
	collision_(0),
	searchLevel_(0),
	direction(8) {

}

void KStar::setSearchLevel(int searchLevel) {
	searchLevel_ = searchLevel;
}

void KStar::addCollision(KStar::Vec2 pos) {
	collision_(pos.x, pos.y) = true;
}

void KStar::removeCollision(KStar::Vec2 pos) {
	collision_(pos.x, pos.y) = false;
}

void KStar::clearCollisions() {
	collision_.fill(false);
}

void KStar::setHeuristic(HeuristicFunction function) {
	heuristic_ = std::bind(function, std::placeholders::_1, std::placeholders::_2);
}

void KStar::setDiagonalMovement(bool enable_) {
	direction = enable_ ? 8 : 4;
}

KStar::Path KStar::searchPath(KStar::Vec2 source, KStar::Vec2 target) {

	Path path(searchLevel_);

	openMap_.push_back(Node(source));
	for (; !openMap_.empty() || path.size() == searchLevel_ ;) {
		Node currentNode = getLowestNode();
		for (int idxDir = 0; idxDir < direction; ++idxDir) {
		}
	}
	return path;
}

bool KStar::isOverflow(KStar::Vec2 v) {
	return v.x < 0 || v.x >= worldSize_ || v.y < 0 || v.y >= worldSize_;
}

void KStar::setWorldSize(int worldSize) {
	worldSize_ = worldSize;
}

bool KStar::detectCollision(KStar::Vec2 vec) {
	return collision_(vec.x, vec.y);
}

KStar::Node KStar::getLowestNode() {
	return *std::min_element(openMap_.begin(), openMap_.end(),
			[](Node const &lNode, Node const &rNode){
				return lNode.F < rNode.F;
	});
}

/**  HEURISTIC **/

KStar::Vec2 KStar::Heuristic::getDelta(KStar::Vec2 source, KStar::Vec2 target) {
	return Vec2();
}

double KStar::Heuristic::manhattan(KStar::Vec2 source, KStar::Vec2 target) {
	auto delta = std::move(KStar::Heuristic::getDelta(source, target));
	return static_cast<uint>(10 * (delta.x + delta.y));
}

double KStar::Heuristic::euclidean(KStar::Vec2 source, KStar::Vec2 target) {
	return std::sqrt(std::pow(source.x - target.x, 2) + std::pow(source.y - target.y, 2));
}
