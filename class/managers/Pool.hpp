//
// Created by Alexandre ROULIN on 22/10/2018.
//

#ifndef NIBBLER_POOL_HPP
#define NIBBLER_POOL_HPP

#include <vector>
#include "IPool.hpp"
#include <components/Component.hpp>
template<typename T>
class Pool : IPool {
public:
	explicit Pool(unsigned int size);

	void resize(unsigned int size);

	unsigned int size();

	virtual void clear();

	T &get(unsigned int index);

	bool set(unsigned int index, T element);

	virtual ~Pool();

private:
	std::vector<T> _pool;
};

template<typename T>
T &Pool<T>::get(unsigned int index) {
	assert(index < _pool.size());
	return _pool[index];
}

template<typename T>
bool Pool<T>::set(unsigned int index, T element) {
	assert(index < _pool.size());
	_pool[index] = element;
	return true;
}

template<typename T>
Pool<T>::Pool(unsigned int size) {
	resize(size);
}

template<typename T>
void Pool<T>::resize(unsigned int size) {
	_pool.resize(size);
}

template<typename T>
Pool<T>::~Pool() {

}

template<typename T>
void Pool<T>::clear() {
	_pool.clear();
}

template<typename T>
unsigned int Pool<T>::size() {
	return static_cast<unsigned int>(_pool.size());
}

#endif //NIBBLER_POOL_HPP
