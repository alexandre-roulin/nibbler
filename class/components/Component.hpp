//
// Created by Alexandre ROULIN on 22/10/2018.
//

#ifndef NIBBLER_COMPONENT_HPP
#define NIBBLER_COMPONENT_HPP

template <typename ComponentType>
struct Component {
	Component() = delete;
	explicit Component(unsigned int sign) : signature(sign) {};
	unsigned int const signature;
};

#endif //NIBBLER_COMPONENT_HPP
