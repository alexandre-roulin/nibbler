#pragma once


#include <KNU/utils/Signature.hpp>
#include "ISystem.hpp"

namespace KNU {

	class MotionSystem : public ISystem {
	public:
		Signature mask;

		template<typename T>
		void requireComponent() {
			mask.addComponent<T>();
		}

		void update() final;

		~MotionSystem() final;

	};

}