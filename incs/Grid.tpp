#pragma once
#include <memory>

template <typename T>
class Grid {
public:

	Grid(size_t rows, size_t columns);
	~Grid(void);

	size_t		getRows(void) const;
	size_t		getColumns(void) const;

	void		setBorder(T const &border);
	void		fill(T const &fill);
	void		clear(void);

	int			*operator[](size_t row);
	int			&operator()(size_t row, size_t column);

	int			*operator[](size_t row) const;
	int			&operator()(size_t row, size_t column) const;

private:
	size_t						_rows;
	size_t						_columns;
	T							*_grid;


	Grid		&operator=(Grid const &rhs);
	Grid(Grid const &src);
	Grid(void);
};


template <typename T>
Grid<T>::Grid(void)
{
	return ;
}

template <typename T>
Grid<T>::~Grid(void)
{
	delete [] this->_grid;
	return ;
}

template <typename T>
Grid<T>::Grid(size_t rows, size_t columns) :
	_rows(rows),
	_columns(columns),
	_grid(new T[rows * columns]())
{}


template <typename T>
size_t		Grid<T>::getRows(void) const
{
	return (this->_rows);
}

template <typename T>
size_t		Grid<T>::getColumns(void) const
{
	return (this->_rows);
}

template <typename T>
void		Grid<T>::setBorder(T const &border)
{
	for (size_t i = 0; i < this->_columns; i++)
	{
		this->_grid[i * this->_rows] = border;
		this->_grid[i * this->_rows + this->_columns - 1] = border;
	}
	for (size_t i = 0; i < this->_rows; i++)
	{
		this->_grid[i] = border;
		this->_grid[i + this->_rows * (this->_rows - 1)] = border;
	}
}
template <typename T>
void		Grid<T>::fill(T const &fill)
{
	for (size_t i = 0; i < this->_rows * this->_columns; i++)
		this->_grid[i] = fill;
}
template <typename T>
void		Grid<T>::clear(void)
{
	for (size_t i = 0; i < this->_rows * this->_columns; i++)
		this->_grid[i] = -1;
}


template <typename T>
int			*Grid<T>::operator[](size_t row)
{
	return (this->_grid + (row * this->_columns));
}

template <typename T>
int			&Grid<T>::operator()(size_t row, size_t column)
{
	return (this->_grid[row * this->_columns + column]);
}

template <typename T>
int			*Grid<T>::operator[](size_t row) const
{
	return (this->_grid + (row * this->_columns));
}

template <typename T>
int			&Grid<T>::operator()(size_t row, size_t column) const
{
	return (this->_grid[row * this->_columns + column]);
}
