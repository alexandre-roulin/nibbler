#pragma once

#include <unordered_map>
#include <typeindex>
#include <KNU/systems/ISystem.hpp>

namespace KNU {
	class SystemManager {
	private:
		std::unordered_map<std::type_index, std::shared_ptr<ISystem>> _systems;
	public:
		SystemManager() {}



		template<typename T>
		void removeSystem() {
			assert(hasSystem<T>());
			_systems.erase(std::type_index(typeid(T)));
		}

		template<typename T>
		T *getSystem() {
			assert(hasSystem<T>());
			auto it = _systems.at(std::type_index(typeid(T)));
			return (std::static_pointer_cast<T>(it).get());
		}

		template<typename T>
		void addSystem() {
			assert(!hasSystem<T>());
			_systems.insert(
					std::make_pair(std::type_index(typeid(T)), new T()));
		}

		template<typename T>
		bool hasSystem() {
			return (_systems.find(std::type_index(typeid(T))) !=
					_systems.end());
		}
	};


}
