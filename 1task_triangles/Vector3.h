#pragma once

#include <iostream>


//! Point template structure
//! with overloaded operator==
template <typename T>
class Vector3 {
public:
	static const float eps;

	T x_;
	T y_;
	T z_;

	Vector3();
	Vector3(T x, T y, T z);

	//! Print 'x' and 'y'
	void print() const;

	static Vector3<T> cross_product(const Vector3<T> &first, const Vector3<T> &second);
	static T scalar_product(const Vector3<T> &first, const Vector3<T> &second);

	bool is_zero();

	friend inline bool operator==(const Vector3<T> &rpoint, const Vector3<T> &lpoint);
	inline Vector3<T> operator-(const Vector3<T> &that) const;
};

template<typename T>
const float Vector3<T>::eps = 1E-09;

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

template<typename T>
inline Vector3<T> Vector3<T>::cross_product(const Vector3<T> &first,
		const Vector3<T> &second) {
	Vector3<T> result;

	result.x_ = first.y_ * second.z_ - first.z_ * second.y_;
	result.y_ = first.z_ * second.x_ - first.x_ * second.z_;
	result.z_ = first.x_ * second.y_ - first.y_ * second.x_;

	return result;
}

template<typename T>
inline T Vector3<T>::scalar_product(const Vector3<T> &first,
		const Vector3<T> &second) {
	return (first.x_ * second.x_ + first.y_ * second.y_ + first.z_ * second.z_);
}

template<typename T>
inline bool Vector3<T>::is_zero() {
	return (x_ * x_ + y_ * y_ + z_ * z_ < eps);

}

template<typename T>
inline Vector3<T> Vector3<T>::operator-(const Vector3<T> &that) const {
	Vector3<T> result;
	result.x_ = x_ - that.x_;
	result.y_ = y_ - that.y_;
	result.z_ = z_ - that.z_;

	return result;
}


