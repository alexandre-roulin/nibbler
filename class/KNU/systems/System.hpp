#pragma once

#include <vector>
#include <KNU/utils/Signature.hpp>
#include <KNU/entities/Entity.hpp>

namespace KNU {
	class SystemManager;

	class World;

	class System {
	public:

		// what component types the system requires of entities (we can use this method in the constructor for example)
		template<typename T>
		void requireComponent();

		virtual void update() = 0;

		// returns a list of entities that the system should process each frame
		std::vector<Entity> getEntities();

		// adds an entity of interest
		void addEntity(Entity e);

		// if the entity is not alive anymore (during processing), the entity should be removed
		void removeEntity(Entity e);

		const Signature &getSignature() const;

		virtual ~System() = default;

	protected:
		World &getWorld() const;

	private:
		// which components an entity must have in order for the system to process the entity
		Signature signature;

		// vector of all entities that the system is interested in
		std::vector<Entity> entities;
		World *world = nullptr;

	};
}