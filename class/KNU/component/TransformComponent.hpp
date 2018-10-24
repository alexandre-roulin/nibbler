#ifndef NIBBLER_TRANSFORMCOMPONENT_HPP
#define NIBBLER_TRANSFORMCOMPONENT_HPP

#include "Component.hpp"

namespace KNU {

	struct TransformComponent : public Component<TransformComponent> {
		explicit TransformComponent(int y = 0, int x = 0) : y(y), x(x) {};
		int y, x;
	};
}


#endif //NIBBLER_TRANSFORMCOMPONENT_HPP
