#pragma once

#include <vector>
#include <KNU/utils/Signature.hpp>
#include <KNU/entities/Entity.hpp>
#include <unordered_map>
#include <typeindex>

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
		friend class SystemManager;
		// which components an entity must have in order for the system to process the entity
		Signature signature;

		// vector of all entities that the system is interested in
		std::vector<Entity> entities;
		World *world = nullptr;

	};
	class SystemManager {
	protected:
		std::unordered_map<std::type_index, std::shared_ptr<System>> _systems;
		World &_world;
	public:
		explicit SystemManager(World &world);

		void addToSystems(Entity &entity);

		template<typename T>
		void removeSystem() {
			if (hasSystem<T>())
				_systems.erase(std::type_index(typeid(T)));
		}

		template<typename T>
		T *getSystem() {
			if (!hasSystem<T>())
				return nullptr;
			auto it = _systems.at(std::type_index(typeid(T)));
			return (std::static_pointer_cast<T>(it).get());
		}

		template<typename T, typename ... Args>
		void addSystem(Args ... args) {
			assert(!hasSystem<T>());
			std::shared_ptr<T> system(new T(std::forward<Args>(args) ...));
			system->world = &_world;
			_systems.insert(
					std::make_pair(std::type_index(typeid(T)), system));
		}

		template<typename T>
		bool hasSystem() {
			return (_systems.find(std::type_index(typeid(T))) !=
					_systems.end());
		}
	};
}