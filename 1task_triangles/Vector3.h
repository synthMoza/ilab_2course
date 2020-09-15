#pragma once

#include <iostream>


//! Point template structure
//! with overloaded operator==
template <typename T>
struct Vector3 {
	T x_;
	T y_;
	T z_;

	Vector3();
	Vector3(T x, T y, T z);

	//! Print 'x' and 'y'
	void print() const;
	friend inline bool operator==(const Vector3<T> &rpoint, const Vector3<T> &lpoint);
};

template <typename T>
inline bool operator==(const Vector3<T>& rpoint, const Vector3<T>& lpoint) {
	if (rpoint.x == lpoint.x && rpoint.y == lpoint.y)
		return true;
	else
		return false;
}

template<typename T>
inline Vector3<T>::Vector3() :
	x_ (0),
	y_ (0),
	z_ (0) {}

template<typename T>
inline Vector3<T>::Vector3(T x, T y, T z) :
	x_ (x),
	y_ (y),
	z_ (z) {}

template<typename T>
inline void Vector3<T>::print() const {
	std::cout << "Point: x = " << x_ << ", y = " << y_ << ", z = " << z_<< "\n";
}
