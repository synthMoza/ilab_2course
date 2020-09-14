#pragma once

#include <iostream>


//! Point template structure
//! with overloaded operator==
template <typename T>
struct Point {
	T x_;
	T y_;

	Point();
	Point(T x, T y);

	//! Print 'x' and 'y'
	void print();
	friend inline bool operator==(const Point& rpoint, const Point& lpoint) const;
};

template <typename T>
inline bool operator==(const Point<T>& rpoint, const Point<T>& lpoint) const {
	if (rpoint.x == lpoint.x && rpoint.y == lpoint.y)
		return true;
	else
		return false;
}

template<typename T>
inline Point<T>::Point() :
	x_ (0),
	y_ (0) {}

template<typename T>
inline Point<T>::Point(T x, T y) :
	x_ (x),
	y_ (y) {}

template<typename T>
inline void Point<T>::print() {
	std::cout << "Point: x = " << x_ << ", y = " << y_ << "\n";
}
