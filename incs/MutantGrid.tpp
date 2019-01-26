#ifndef NIBBLER_MUTANTGRID_HPP
#define NIBBLER_MUTANTGRID_HPP

#include <vector>

template<typename T>
class MutantGrid : public std::vector<T> {
public:
	MutantGrid();

	MutantGrid(size_t size);

	void fill(T const &fill);

	typename std::vector<T>::reference operator()(size_t x, size_t y);
	typename std::vector<T>::const_reference operator()(size_t x, size_t y) const;

	std::pair<size_t, size_t> getRandomSlot(T value);

	bool isAnyFreeSlotInRow(size_t row, T clear) const;

	bool isFreeSlot(size_t row, size_t column, T clear) const;

private:
	size_t size_;
};

template<typename T>
typename std::vector<T>::reference MutantGrid<T>::operator()(size_t x, size_t y) {
	assert(y * size_ + x < this->size());
	return this->at (y * size_ + x );
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
void MutantGrid<T>::fill(const T &fill) {
//	std::for_each(std::vector<T>::begin(), std::vector<T>::end(), [fill](T &value){ value = fill; });
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
					assert(std::vector<T>::__begin_[base_y * size_ + base_x] == value);
					return std::make_pair(base_x, base_y);
				}
			}
	}

}

template<typename T>
bool MutantGrid<T>::isAnyFreeSlotInRow(size_t row, T free) const {
	for (int index = 0; index < size_; ++index) {
		if (std::vector<T>::begin()[row * size_ + index] == free)
			return true;
	}
	return false;
}

template<typename T>
bool MutantGrid<T>::isFreeSlot(size_t row, size_t column, T clear) const {
	return std::vector<T>::begin()[row * size_ + column] == clear;
}

template<typename T>
MutantGrid<T>::MutantGrid() {

}


#endif //NIBBLER_MUTANTGRID_HPP
