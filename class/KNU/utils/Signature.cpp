//
// Created by Alexandre ROULIN on 24/10/2018.
//

#include "Signature.hpp"

bool KNU::Signature::matches(KNU::Signature systemMask) {
	return ((mask & systemMask.mask) == systemMask.mask);
}