#ifndef PROJECT_SNAKENETWORK_HPP
#define PROJECT_SNAKENETWORK_HPP

class ISnakeNetwork {

	void changeIsBorderless(bool borderless);
	void changeMapSize(unsigned int);
	const SnakeArrayContainer &getSnakeArray_() const;
	bool allSnakeIsDead() const;
	bool allSnakeIsReady() const;

};

#endif //PROJECT_SNAKENETWORK_HPP
