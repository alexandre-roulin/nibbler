#pragma once

namespace KNU {

	struct Entity {
		Entity() : id(0) {};
		int id;

		bool operator<(const Entity &rhs) const {
			return id < rhs.id;
		}

		bool operator>(const Entity &rhs) const {
			return rhs < *this;
		}

		bool operator<=(const Entity &rhs) const {
			return !(rhs < *this);
		}

		bool operator>=(const Entity &rhs) const {
			return !(*this < rhs);
		}
	};

}
