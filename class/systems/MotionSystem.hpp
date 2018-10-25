#pragma once


#include <KNU/utils/Signature.hpp>
#include "KNU/systems/System.hpp"

namespace KNU {

	class MotionSystem : public System {
	public:
		virtual void update();

		~MotionSystem() final;

	};

}