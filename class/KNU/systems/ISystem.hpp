#pragma once

class ISystem {
public:
	virtual void update() = 0;
	virtual ~ISystem() {};
};