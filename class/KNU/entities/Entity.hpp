#pragma once

#include <KNU/utils/Signature.hpp>
#include <string>
#include <ostream>

namespace KNU {

	struct Entity {
	private:
	public:
		Entity() : id(-1) {};


		Entity(unsigned int id)
				: id(id),
				  alive(true) {
		};
		int id;
		Signature mask;
		bool alive;

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

		bool operator==(const Entity &rhs) const {
			return id == rhs.id &&
				   mask == rhs.mask &&
				   alive == rhs.alive;
		}

		friend std::ostream &
		operator<<(std::ostream &os, const Entity &entity) {
			os << "id: " << entity.id << " " << entity.mask << " alive: "
			   << entity.alive;
			return os;
		}

		bool operator!=(const Entity &rhs) const {
			return !(rhs == *this);
		}
	};
}
