#pragma once

#include <ostream>
#include <KNU/component/Component.hpp>

namespace KNU {

	struct Signature {
	private:
		unsigned int mask;
	public:
		explicit Signature(unsigned int = 0);

		template<typename ComponentType>
		void addComponent();

		template<typename ComponentType>
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
	template<typename ComponentType>
	void Signature::addComponent() {
		mask |= (1 << getComponentSignature<ComponentType>());
	}

	template<typename ComponentType>
	void Signature::removeComponent() {
		mask ^= (1 << getComponentSignature<ComponentType>());
	}

}

