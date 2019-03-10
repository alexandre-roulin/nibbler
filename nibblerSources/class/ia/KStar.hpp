#pragma once

#include <vector>
#include <array>
#include <ostream>
#include "MutantGrid.tpp"

class KStar {

public:
	struct Vec2 {
		Vec2(int x = 0, int y = 0);

		Vec2(Vec2 const &);

		bool operator==(const Vec2 &rhs) const;

		friend std::ostream &operator<<(std::ostream &os, const Vec2 &vec2);

		bool operator!=(const Vec2 &rhs) const;

		Vec2 operator+(Vec2 const &lhs);

		Vec2 &operator=(Vec2 const &rhs);

		int x;
		int y;
	};

	struct Node {
		Node(const Vec2 vec = Vec2(), double H = -1, double G = -1,
			 double F = -1, Vec2 parent = Vec2());

		Node(Node const &rhs);

		Node &operator=(Node const &);

		operator bool() const;

		friend std::ostream &operator<<(std::ostream &os, const Node &node);

		bool operator==(const Node &rhs) const;

		bool operator!=(const Node &rhs) const;

		void clear();

		Vec2 vec;
		double H;
		double G;
		double F;
		Vec2 parent;
//		Node *parent;
	};

	using Path = std::vector<Vec2>;
	using HeuristicFunction = std::function<double(Vec2, Vec2)>;

	KStar();
	~KStar() = default;
	KStar &operator=(const KStar &) = delete;
	KStar(const KStar &) = delete;

	void setWorldSize(Vec2);

	void addCollision(Vec2);

	void removeCollision(Vec2);

	void clearCollisions();

	void setHeuristic(HeuristicFunction);

	void setDiagonalMovement(bool);

	bool isOverflow(Vec2);

	bool isCollision(Vec2);

	Path searchPath(Vec2 source, Vec2 target);

	Node getLowestNode();

	class Heuristic {
	public:
		static KStar::Vec2 getDelta(Vec2, Vec2);

		static double manhattan(Vec2, Vec2);

		static double euclidean(Vec2, Vec2);
	};

	void print(Vec2 s, Vec2 t, Path p);

private:
public:
	int getDirection() const;

	static const std::array<Vec2, 8> &getDirections();

private:
	int direction;
	static std::array<Vec2, 8> const directions;
	HeuristicFunction heuristic_;
	MutantGrid<Node> closeMap_;
	MutantGrid<Node> openMap_;
	MutantGrid<bool> collision_;
	Vec2 worldSize_;
};


