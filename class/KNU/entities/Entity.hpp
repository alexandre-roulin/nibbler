#pragma once

#include <string>
#include <ostream>
#include <KNU/utils/Signature.hpp>

namespace KNU {
	class World;
	class EntitiesManager;

	struct Entity {
	private:
		int id;
		Signature mask;
		bool alive;

		Entity(unsigned int id)
				: id(id),
				  alive(true) {
		};
		EntitiesManager *entityManager;

	public:
		Entity() : id(-1) {};

		friend class EntitiesManager;

		const Signature &getMask();

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

		bool operator!=(const Entity &rhs) const {
			return !(rhs == *this);
		}

		EntitiesManager &getEntitiesManager() const;

		void kill();

		bool isAlive() const;

		template<typename T>
		void addComponent(T component);

		template<typename T, typename ... Args>
		void addComponent(Args &&... args);

		template<typename T>
		void removeComponent();

		template<typename T>
		bool hasComponent() const;

		template<typename T>
		T &getComponent() const;

		void tag(std::string tag);

		bool hasTag(std::string tag) const;

		void group(std::string group);

		bool hasGroup(std::string group) const;

		friend std::ostream &
		operator<<(std::ostream &os, const Entity &entity) {
			os << "id: " << entity.id << " " << entity.mask << " alive: "
			   << entity.alive;
			return os;
		}
	};
}
