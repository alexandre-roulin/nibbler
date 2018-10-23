//
// Created by Alexandre ROULIN on 22/10/2018.
//

#ifndef NIBBLER_NIBBLER_HPP
#define NIBBLER_NIBBLER_HPP

enum eSignatureComponent {
	MOTION_COMPONENT = 1 << 1,
	TRANSFORM_COMPONENT = 1 << 2
};

enum eDirection {
	RIGHT,
	LEFT,
	NONE
};

#endif //NIBBLER_NIBBLER_HPP
