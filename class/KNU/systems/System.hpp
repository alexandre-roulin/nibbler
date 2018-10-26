#pragma once

#include <vector>
#include <KNU/utils/Signature.hpp>
#include <unordered_map>
#include <typeindex>
#include <KNU/entities/Entity.hpp>

namespace KNU {
	class SystemManager;
	class World;

	class System {
	public:

		template<typename T>
		void requireComponent();

		virtual void update() = 0;

		std::vector<Entity> getEntities();

		void addEntity(Entity &entity);

		void removeEntity(Entity entity);

		const Signature &getSignature() const;

		virtual ~System() = default;

	protected:
		World &getWorld() const;

	private:
		friend class SystemManager;

		Signature signature;

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

		void removeToSystems(Entity &entity);

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
			_systems.insert(std::make_pair(std::type_index(typeid(T)), system));
		}

		template<typename T>
		bool hasSystem() {
			return (_systems.find(std::type_index(typeid(T))) !=
					_systems.end());
		}
	};

	template<typename T>
	void System::requireComponent() {
		signature.addComponent<T>();
	}

}