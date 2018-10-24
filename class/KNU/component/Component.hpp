#pragma once

#include <type_traits>

namespace KNU {
	struct ComponentCounter {
		static int familyCounter;
	};

	template<typename ComponentType>
	struct Component {
		static inline int signature() {
			static int signature = ComponentCounter::familyCounter++;
			return signature;
		}
	};


	// Util method for getting family given type
	template <typename C>
	static int getComponentSignature() {
		return Component<typename std::remove_const<C>::type>::signature();
	}
}


