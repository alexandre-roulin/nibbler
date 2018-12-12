#pragma once

#include <memory>
#include <iostream>
#include <iomanip>

#define FREE_SLOT -1

template<typename T>
class Grid {
public:

	Grid(size_t size);

	Grid(size_t rows, size_t columns);

	~Grid(void);

	Grid(Grid const &src);

	Grid &operator=(Grid const &rhs);

	size_t getRows(void) const;

	size_t getColumns(void) const;

	void setBorder(T const &border);

	void fill(T const &fill);

	void clear(void);

	std::pair<size_t, size_t> getRandomSlot(T value);
	void print();


	T *operator[](size_t row);

	T &operator()(size_t row, size_t column);

	T *operator[](size_t row) const;

	T &operator()(size_t row, size_t column) const;

private:

	bool isAnyFreeSlotInRow(size_t row, T clear) const;
	bool isFreeSlot(size_t row, size_t column, T clear) const;

	size_t _rows;
	size_t _columns;
	T *_grid;
	size_t size_;

	Grid(void);
};


template<typename T>
Grid<T>::Grid(void) {
	return;
}

template<typename T>
Grid<T>::Grid(size_t rows, size_t columns) :
		_rows(rows),
		_columns(columns),
		_grid(new T[rows * columns]()),
		size_(rows * columns){

}

template<typename T>
Grid<T>::Grid(size_t size)
		:_rows(size),
		 _columns(size),
		 _grid(new T[size * size]()),
		 size_(size * size) {

}

template<typename T>
Grid<T>::Grid(Grid const &src) :
		_rows(src._rows),
		_columns(src._columns),
		_grid(new T[src._rows * src._columns]()),
		size_(src.size_){
	for (size_t i = 0; i < size_; i++)
		this->_grid[i] = src._grid[i];
}

template<typename T>
Grid<T>::~Grid(void) {
	delete[] this->_grid;
	return;
}

template<typename T>
size_t Grid<T>::getRows(void) const {
	return (this->_rows);
}


template<typename T>
size_t Grid<T>::getColumns(void) const {
	return (this->_rows);
}

template<typename T>
void Grid<T>::setBorder(T const &border) {
	for (size_t i = 0; i < this->_columns; i++) {
		this->_grid[i * this->_rows] = border;
		this->_grid[i * this->_rows + this->_columns - 1] = border;
	}
	for (size_t i = 0; i < this->_rows; i++) {
		this->_grid[i] = border;
		this->_grid[i + this->_rows * (this->_rows - 1)] = border;
	}
}

template<typename T>
void Grid<T>::fill(T const &fill) {
	for (size_t i = 0; i < size_; i++)
		this->_grid[i] = fill;
}

template<typename T>
void Grid<T>::clear(void) {
	for (size_t i = 0; i < size_; i++)
		this->_grid[i] = FREE_SLOT;
}

template<typename T>
T *Grid<T>::operator[](size_t row) {
	return (this->_grid + (row * this->_columns));
}

template<typename T>
T &Grid<T>::operator()(size_t column, size_t row) {
	return (this->_grid[row * this->_columns + column]);
}

template<typename T>
T *Grid<T>::operator[](size_t row) const {
	return (this->_grid + (row * this->_columns));
}

template<typename T>
T &Grid<T>::operator()(size_t column, size_t row) const {
	return (this->_grid[row * this->_columns + column]);
}

template<typename T>
Grid<T> &Grid<T>::operator=(Grid<T> const &src) {
	if (this->_grid)
		delete[] _grid;
	this->_columns = src._columns;
	this->_rows = src._rows;
	size_ = src.size_;
	this->_grid = new T[src._columns * src._rows];
	for (size_t i = 0; i < size_; ++i)
		this->_grid[i] = src._grid[i];
	return (*this);
}

template<typename T>
std::pair<size_t, size_t> Grid<T>::getRandomSlot(T value) {

	size_t rand_y = rand() % _rows + 1;
	size_t rand_x = rand() % _columns + 1;

	for (int base_y = 0;; ++base_y) {
		if (base_y >= _rows)
			base_y = 0;
		if (isAnyFreeSlotInRow(base_y, value))
			rand_y--;
		if (rand_y == 0)
			for (int base_x = 0;; ++base_x) {
				if (base_x >= _columns)
					base_x = 0;
				if (isFreeSlot(base_y, base_x, value))
					--rand_x;
				if (rand_x == 0) {
					print();
					return std::make_pair(base_x, base_y);
				}
			}
	}

}

template<typename T>
bool Grid<T>::isAnyFreeSlotInRow(size_t row, T free) const {
	for (int index = 0; index < _columns; ++index) {
		if (_grid[row * _columns + index] == free)
			return true;
	}
	return false;
}

template<typename T>
bool Grid<T>::isFreeSlot(size_t row, size_t column, T clear) const {
	return _grid[row * _columns + column] == clear;
}
//y 1 x 1
template<typename T>
void Grid<T>::print() {
	for (int y = 0; y < _rows; ++y) {
		for (int x = 0; x < _columns; ++x) {
			std::cout << std::setw(4) << _grid[_rows * y + x];
		}
		std::cout << std::endl;
	}
}
