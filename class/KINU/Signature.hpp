#pragma once

#include <ostream>
#include "Component.hpp"

namespace KINU {

	struct Signature {
	private:
		unsigned int mask;
	public:
		Signature(unsigned int = 0);

		template<typename T>
		void addComponent();

		template<typename T>
		void removeComponent();

		void clean();

		bool matches(Signature systemMask) const;

		Signature &operator=(Signature const &signature);

		bool operator==(const Signature &rhs) const;

		bool operator!=(const Signature &rhs) const;

		unsigned int getMask() const;

		friend std::ostream &
		operator<<(std::ostream &os, const Signature &signature);
	};
	template<typename T>
	void Signature::addComponent() {
		mask |= (1 << getComponentSignature<T>());
	}

	template<typename T>
	void Signature::removeComponent() {
		mask ^= (1 << getComponentSignature<T>());
	}

}

