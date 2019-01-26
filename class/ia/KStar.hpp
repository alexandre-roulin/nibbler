#include "Grid.tpp"
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
		int x;
		int y;
	};

	struct Node {
		Node(const Vec2 vec = Vec2(), double H = 0, double G = 0, double F = 0);

		operator bool() const;

		bool operator==(const Node &rhs) const;

		bool operator!=(const Node &rhs) const;

		Vec2 vec;
		double H;
		double G;
		double F;
		Node *parent;
	};

	using Path = std::vector<Vec2>;
	using HeuristicFunction = std::function<double (Vec2,Vec2)>;

	KStar();
	void setSearchLevel(int);
	void setWorldSize(int);
	void addCollision(Vec2);
	void removeCollision(Vec2);
	void clearCollisions();
	bool detectCollision(Vec2);
	void setHeuristic(HeuristicFunction);
	void setDiagonalMovement(bool);
	bool isOverflow(Vec2);
	Path searchPath(Vec2 source, Vec2 target);
	Node getLowestNode();
	class Heuristic {
	public:
		static KStar::Vec2 getDelta(Vec2, Vec2);

		static double manhattan(Vec2, Vec2);
		static double euclidean(Vec2, Vec2);
	};
private:
	static std::array<Vec2, 8> const directions;
	HeuristicFunction heuristic_;
	MutantGrid<Node> closeMap_;
	MutantGrid<Node> openMap_;
	MutantGrid<bool> collision_;
	int direction;
	int searchLevel_;
	int worldSize_;
};


