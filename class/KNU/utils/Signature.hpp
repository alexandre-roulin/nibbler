#pragma once

#include <KNU/component/Component.hpp>

namespace KNU {

	struct Signature {
		unsigned int mask = 0;

		template<typename ComponentType>
		void addComponent() {
			mask |= (1 << getComponentSignature<ComponentType>());
		}

		template<typename ComponentType>
		void removeComponent() {
			mask ^= (1 << getComponentSignature<ComponentType>());
		}

		bool matches(Signature systemMask);
	};

}

