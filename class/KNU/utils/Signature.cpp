//
// Created by Alexandre ROULIN on 24/10/2018.
//

#include "Signature.hpp"

bool KNU::Signature::matches(KNU::Signature systemMask)const {
	return ((mask & systemMask.mask) == systemMask.mask);
}

unsigned int KNU::Signature::getMask() const {
	return mask;
}

bool KNU::Signature::operator==(const KNU::Signature &rhs) const {
	return mask == rhs.mask;
}

bool KNU::Signature::operator!=(const KNU::Signature &rhs) const {
	return !(rhs == *this);
}

std::ostream &
KNU::operator<<(std::ostream &os, const KNU::Signature &signature) {
	os << "mask: " << signature.mask;
	return os;
}

