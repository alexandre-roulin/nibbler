//
// Created by Alexandre ROULIN on 22/10/2018.
//

#ifndef NIBBLER_IPOOL_HPP
#define NIBBLER_IPOOL_HPP

class IPool {
public:
	virtual void clear() = 0;
	virtual ~IPool() = default;
};

#endif //NIBBLER_IPOOL_HPP
