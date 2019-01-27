#ifndef NIBBLER_MUTANTGRID_HPP
#define NIBBLER_MUTANTGRID_HPP

#include <logger.h>
#include <vector>

template<typename T>
class MutantGrid : public std::vector<T> {
public:
	MutantGrid();

	MutantGrid(size_t size);
	MutantGrid(size_t , size_t);

	void fill(T const &fill);

	typename std::vector<T>::reference operator()(size_t x, size_t y);
	typename std::vector<T>::const_reference operator()(size_t x, size_t y) const;

	std::pair<size_t, size_t> getRandomSlot(T value);

	void resize(size_t size);

	bool isAnyFreeSlotInRow(size_t row, T clear) const;

	bool isFreeSlot(size_t row, size_t column, T clear) const;

private:
	size_t size_;
};

template<typename T>
typename std::vector<T>::reference MutantGrid<T>::operator()(size_t x, size_t y) {
	if (y * size_ + x >= std::vector<T>::size())
//		log_error("Test : %d , x : %d  y : %d s : %d", y * size_ + x, x, y, size_);
	assert(y * size_ + x < std::vector<T>::size());
	return this->at(y * size_ + x );
}


template<typename T>
typename std::vector<T>::const_reference MutantGrid<T>::operator()(size_t x, size_t y) const {
	assert(y * size_ + x < this->size());
	return this->at(y * size_ + x );
}

template<typename T>
MutantGrid<T>::MutantGrid(size_t size):
	std::vector<T>(size * size),
	size_(size) {

}
template<typename T>
MutantGrid<T>::MutantGrid(size_t size1, size_t size2):
		std::vector<T>(size1 * size2),
		size_(size1) {

}
template<typename T>
void MutantGrid<T>::fill(const T &fill) {
	for (int idx = 0; idx < this->size(); ++idx) {
		this->at(idx) = fill;
	}
}

template<typename T>
std::pair<size_t, size_t> MutantGrid<T>::getRandomSlot(T value) {

	size_t rand_y = rand() % size_ + 1;
	size_t rand_x = rand() % size_ + 1;

	for (int base_y = 0;; ++base_y) {
		if (base_y >= size_)
			base_y = 0;
		if (isAnyFreeSlotInRow(base_y, value))
			rand_y--;
		if (rand_y == 0)
			for (int base_x = 0;; ++base_x) {
				if (base_x >= size_)
					base_x = 0;
				if (isFreeSlot(base_y, base_x, value))
					--rand_x;
				if (rand_x == 0) {
					assert(this->at(base_y * size_ + base_x) == value);
					return std::make_pair(base_x, base_y);
				}
			}
	}

}

template<typename T>
bool MutantGrid<T>::isAnyFreeSlotInRow(size_t row, T free) const {
	for (int index = 0; index < size_; ++index) {
		if (this->at(row * size_ + index) == free)
			return true;
	}
	return false;
}

template<typename T>
bool MutantGrid<T>::isFreeSlot(size_t row, size_t column, T clear) const {
	return this->at(row * size_ + column) == clear;
}

template<typename T>
MutantGrid<T>::MutantGrid() {

}

template<typename T>
void MutantGrid<T>::resize(size_t size) {
	size_ = size;
	std::vector<T>::resize(size * size);
}




#endif //NIBBLER_MUTANTGRID_HPP
