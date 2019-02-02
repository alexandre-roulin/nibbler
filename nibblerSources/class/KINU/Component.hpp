#pragma once

#include <cstdint>
#include <bitset>
#include "Config.hpp"
#include <cassert>

namespace KINU {

	struct BaseComponent {
	public:
		using ID = uint8_t;
	protected:
		static ID nextId;
	};

	template <typename T>
	struct Component : BaseComponent {
		static ID getId();
	};

	template<typename T>
	BaseComponent::ID Component<T>::getId() {
		static ID id = nextId++;
		assert(id < eConfig::MAX_COMPONENT);
		return id;
	}

	using ComponentMask = std::bitset<eConfig::MAX_COMPONENT>;
}