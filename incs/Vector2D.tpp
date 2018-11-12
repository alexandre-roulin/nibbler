#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP

#include <iostream>
#include <stdlib.h>

template <typename T>
class Vector2D {
    public:

        Vector2D(void) : _x(T()), _y(T()) {};
        Vector2D(T const &x, T const &y) : _x(x), _y(y) {};
        Vector2D(T const nb) : _x(nb), _y(nb) {};
        Vector2D(Vector2D const &rhs) {*this = rhs;};
        ~Vector2D(void) {};

        T const   &getX(void) const {return (this->_x);}

        T const   &getY(void) const {return (this->_y);}

        void            setX(T const &x) {this->_x = x;};
        void            setY(T const &y) {this->_y = y;};


        T       &operator=(T const &rhs) {
            this->_x = rhs._x;
            this->_y = rhs._y;
        }

        Vector2D operator+(Vector2D const &rhs) const {
            return (Vector2D(this->_x + rhs._x, this->_y + rhs._y));
        }

        Vector2D operator-(Vector2D const &rhs) const {
            return (Vector2D(this->_x - rhs._x, this->_y - rhs._y));
        }

        Vector2D operator*(Vector2D const &rhs) const {
            return (Vector2D(this->_x * rhs._x, this->_y * rhs._y));
        }

        Vector2D operator*(int const rhs) const {
            return (Vector2D(this->_x * rhs, this->_y * rhs));
        }

        Vector2D operator*(float const rhs) const {
            return (Vector2D(this->_x * rhs, this->_y * rhs));
        }

        Vector2D operator/(Vector2D const &rhs) const {
            return (Vector2D(this->_x / rhs._x, this->_y / rhs._y));
        }

        Vector2D operator/(int const rhs) const {
            return (Vector2D(this->_x / rhs, this->_y / rhs));
        }

        Vector2D operator/(float const rhs) const {
            return (Vector2D(this->_x / rhs, this->_y / rhs));
        }

        Vector2D        &operator+=(Vector2D const &rhs) {
            this->_x += rhs._x;
            this->_y += rhs._y;
            return (*this);
        }

        Vector2D        &operator-=(Vector2D const &rhs) {
            this->_x -= rhs._x;
            this->_y -= rhs._y;
            return (*this);
        }

        Vector2D        &operator*=(Vector2D const &rhs) {
            this->_x *= rhs._x;
            this->_y *= rhs._y;
            return (*this);
        }

        Vector2D        &operator*=(int rhs) {
            this->_x *= rhs;
            this->_y *= rhs;
            return (*this);
        }

        Vector2D        &operator*=(float rhs) {
            this->_x *= rhs;
            this->_y *= rhs;
            return (*this);
        }

        Vector2D        &operator/=(Vector2D const &rhs) {
            this->_x /= rhs._x;
            this->_y /= rhs._y;
            return (*this);
        }

        Vector2D        &operator/=(int rhs) {
            this->_x /= rhs;
            this->_y /= rhs;
            return (*this);
        }

        Vector2D        &operator/=(float rhs) {
            this->_x /= rhs;
            this->_y /= rhs;
            return (*this);
        }

        Vector2D<int>   toInt(void) const {
            return (Vector2D<int>(int(this->_x), int(this->_y)));
        }

        Vector2D<float> toFloat(void) const {
            return (Vector2D<float>(float(this->_x), float(this->_y)));
        }

        static Vector2D<float> toFloat(Vector2D<int> &var) {
            return (Vector2D<float>(float(var._x), float(var._y)));
        };
        static Vector2D<int> toInt(Vector2D<float> &var) {
            return (Vector2D<int>(int(var._x), int(var._y)));
        };

        static Vector2D<float>  random(void) {
            return (Vector2D((float) rand() / ((float) RAND_MAX),
                    (float) rand() / ((float) RAND_MAX)));
        }

        static Vector2D<int>    random(int max) {
            return (Vector2D(rand() % max, rand() % max));
        }

    private:

        T _x;
        T _y;

        static const bool debug;
};

template <typename T>
std::ostream        &operator<<(std::ostream &o, Vector2D<T> const &vec) {
    o << "X[" << vec.getX() << "] Y[" << vec.getY() << "]" << std::endl;
    return (o);
}

#endif
