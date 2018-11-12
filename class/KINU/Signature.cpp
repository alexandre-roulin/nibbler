//
// Created by Alexandre ROULIN on 24/10/2018.
//

#include "Signature.hpp"
#include <iostream>

namespace KINU {

	bool Signature::matches(Signature systemMask) const {
		return ((mask & systemMask.mask) == systemMask.mask);
	}

	unsigned int Signature::getMask() const {
		return mask;
	}

	bool Signature::operator==(const Signature &rhs) const {
		return mask == rhs.mask;
	}

	bool Signature::operator!=(const Signature &rhs) const {
		return !(rhs == *this);
	}

	std::ostream &
	operator<<(std::ostream &os, const Signature &signature) {
		os << "mask: " << signature.mask;
		return os;
	}

	void Signature::clean() {
		mask = 0;
	}

	Signature &Signature::operator=(Signature const &signature) {
		if (this != &signature) {
			mask = signature.mask;
		}
		return *this;
	}

	Signature::Signature(unsigned int mask) : mask(mask) {}


}