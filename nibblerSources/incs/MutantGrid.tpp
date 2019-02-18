#ifndef NIBBLER_MUTANTGRID_HPP
#define NIBBLER_MUTANTGRID_HPP

#include <logger.h>
#include <vector>
#include <assert.h>
#include <iostream>

template<typename T>
class MutantGrid : public std::vector<T> {
public:
	MutantGrid() = delete;

	MutantGrid(size_t size);

	MutantGrid(size_t, size_t);

	MutantGrid<T> &operator=(MutantGrid<T> const &);

	void fill(T const &fill);

	typename std::vector<T>::reference operator()(size_t x, size_t y);

	typename std::vector<T>::const_reference
	operator()(size_t x, size_t y) const;

	std::pair<size_t, size_t> getRandomSlot(T value);

	bool isAnyFreeSlotInRow(size_t row, T clear) const;

	bool isFreeSlot(size_t row, size_t column, T clear) const;

private:
	size_t x_max_;
	size_t y_max_;
};


template<typename T>
MutantGrid<T> &MutantGrid<T>::operator=(MutantGrid<T> const &rvalue) {
	this->resize(rvalue.size());
	assert(this->size() == rvalue.size());
	y_max_ = rvalue.y_max_;
	x_max_ = rvalue.x_max_;
	for (size_t index = 0; index < rvalue.size(); ++index) {
		this->at(index) = rvalue.at(index);
	}
	return *this;
}

template<typename T>
typename std::vector<T>::reference
MutantGrid<T>::operator()(size_t x, size_t y) {
	if (y * y_max_ + x >= std::vector<T>::size())
//		log_error("Test : %d , x : %d  y : %d s : %d", y * size_ + x, x, y, size_);
		assert(y * y_max_ + x < std::vector<T>::size());
	return this->at(y * y_max_ + x);
}


template<typename T>
typename std::vector<T>::const_reference
MutantGrid<T>::operator()(size_t x, size_t y) const {
	assert(y * y_max_ + x < this->size());
	return this->at(y * y_max_ + x);
}

template<typename T>
MutantGrid<T>::MutantGrid(size_t size):
		std::vector<T>(size * size),
		x_max_(size),
		y_max_(size) {

}

template<typename T>
MutantGrid<T>::MutantGrid(size_t max_x, size_t max_y):
		std::vector<T>(max_y * max_x),
		x_max_(max_x),
		y_max_(max_y) {


}

template<typename T>
void MutantGrid<T>::fill(const T &fill) {
	assert(this->size() == y_max_ * x_max_);
	for (size_t idx = 0; idx < this->size(); ++idx) {
		this->at(idx) = fill;
	}
}

template<typename T>
std::pair<size_t, size_t> MutantGrid<T>::getRandomSlot(T value) {

	size_t rand_y = rand() % y_max_ + 1;
	size_t rand_x = rand() % x_max_ + 1;

	for (size_t base_y = 0;; ++base_y) {
		if (base_y >= y_max_)
			base_y = 0;
		if (isAnyFreeSlotInRow(base_y, value))
			rand_y--;
		if (rand_y == 0)
			for (size_t base_x = 0;; ++base_x) {
				if (base_x >= x_max_)
					base_x = 0;
				if (isFreeSlot(base_y, base_x, value))
					--rand_x;
				if (rand_x == 0) {
					assert(this->at(base_y * y_max_ + base_x) == value);
					return std::make_pair(base_x, base_y);
				}
			}
	}

}

template<typename T>
bool MutantGrid<T>::isAnyFreeSlotInRow(size_t row, T free) const {
	for (size_t index = 0; index < x_max_; ++index) {
		if (this->at(row * y_max_ + index) == free)
			return true;
	}
	return false;
}

template<typename T>
bool MutantGrid<T>::isFreeSlot(size_t y, size_t x, T clear) const {
	return this->at(y * y_max_ + x) == clear;
}


#endif //NIBBLER_MUTANTGRID_HPP
