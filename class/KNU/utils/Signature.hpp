#pragma once

#include <KNU/component/Component.hpp>
#include <ostream>

namespace KNU {

	struct Signature {
	private:
		unsigned int mask;
	public:
		explicit Signature(unsigned int mask = 0) : mask(mask) {};

		template<typename ComponentType>
		void addComponent() {
			mask |= (1 << getComponentSignature<ComponentType>());
		}

		template<typename ComponentType>
		void removeComponent() {
			mask ^= (1 << getComponentSignature<ComponentType>());
		}

		void clean() {
			mask = 0;
		}

		bool matches(Signature systemMask);

		Signature &operator=(Signature const &signature) {
			if (this != &signature) {
				mask = signature.mask;
			}
			return *this;
		}

		bool operator==(const Signature &rhs) const;

		bool operator!=(const Signature &rhs) const;

		unsigned int getMask() const;

	};
	std::ostream &operator<<(std::ostream &os, const Signature &signature);

}

