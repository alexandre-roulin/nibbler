#pragma once

#include <type_traits>

namespace KNU {
	struct ComponentCounter {
		static int familyCounter;
	};

	template<typename>
	struct Component {
		static inline int signature() {
			static int signature = ComponentCounter::familyCounter++;
			return signature;
		}
	};


	template<typename C>
	static int getComponentSignature() {
		return Component<typename std::remove_const<C>::type>::signature();
	}
}


