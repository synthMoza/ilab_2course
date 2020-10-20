#pragma once

#include "Vector3.h"
#include <vector>

namespace mfn {

template<typename T>
class OctoTree;

template<typename T>
class Triangle {
	static const float eps;
	std::vector<Vector3<T>> points_;
	static bool are_projections_collided(const Vector3<T> &base,
			const Triangle<T> &first, const Triangle<T> &second);
public:
	int number;

	Triangle(const std::vector<Vector3<T>> &points);

	void print() const;
	bool is_collided(const Triangle &that) const;

	friend class OctoTree<T>;
};

template<typename T>
const float Triangle<T>::eps = 1E-07;

template<typename T>
inline Triangle<T>::Triangle(const std::vector<Vector3<T> > &points) :
	points_ (points),
	number (0) {}

template<typename T>
inline void Triangle<T>::print() const {
	std::cout << "Triangle: \n";
	for (auto point : points_) {
		point.print();
	}
}

template<typename T>
inline bool Triangle<T>::are_projections_collided(const Vector3<T> &base,
		const Triangle<T> &first, const Triangle<T> &second) {
	T fmin = Vector3<T>::scalar_product(first.points_.at(0), base);
	T fmax = fmin;
	T smin = Vector3<T>::scalar_product(second.points_.at(0), base);
	T smax = smin;

	for (auto point : first.points_) {
		T _scalar = Vector3<T>::scalar_product(point, base);

		if (_scalar - eps > fmax)
			fmax = _scalar;
		if (_scalar < fmin - eps)
			fmin = _scalar;
	}

	for (auto point : second.points_) {
		T _scalar = Vector3<T>::scalar_product(point, base);
		if (_scalar - eps > smax)
			smax = _scalar;
		if (_scalar < smin - eps)
			smin = _scalar;
	}

	if (smin - fmax > eps || fmin - smax > eps) {
		// No collision
		return false;
	} else {
		return true;
	}
}

template<typename T>
inline bool Triangle<T>::is_collided(const Triangle &that) const {
	// All vectors of the first triangle
	std::vector<Vector3<T>> fsides;
	std::vector<Vector3<T>> ssides;
	std::vector<Vector3<T>> n_fsides;
	std::vector<Vector3<T>> n_ssides;
	fsides.resize(3);
	ssides.resize(3);

	fsides.at(0) = points_.at(0) - points_.at(1);
	fsides.at(1) = points_.at(1) - points_.at(2);
	fsides.at(2) = points_.at(2) - points_.at(0);

	// All vectors of the second triangle
	ssides.at(0) = that.points_.at(0) - that.points_.at(1);
	ssides.at(1) = that.points_.at(1) - that.points_.at(2);
	ssides.at(2) = that.points_.at(2) - that.points_.at(0);

	// Normals of the triangles
	Vector3<T> n1 = Vector3<T>::cross_product(fsides.at(0), fsides.at(1));
	Vector3<T> n2 = Vector3<T>::cross_product(ssides.at(0), ssides.at(1));

	// Create vectors from cross product of normal and sides
	for (auto side : fsides)
	{
		Vector3<T> _temp = Vector3<T>::cross_product(side, n1);
		n_fsides.push_back(_temp);
	}

	// Check if the triangle is a point or a line
	if (!Triangle<T>::are_projections_collided({1, 1, 1}, *this, that))
	{
		return false;
	}

	for (auto side : ssides)
	{
		Vector3<T> _temp = Vector3<T>::cross_product(side, n2);
		n_ssides.push_back(_temp);
	}

	// Normals of the first triangle
	if (!Triangle<T>::are_projections_collided(n1, *this, that))
	{
		return false;
	}
	
	// for (auto nside : n_fsides) {
	// 	if (!Triangle<T>::are_projections_collided(nside, *this, that)) {
	// 		return false;
	// 	}
	// }

	// Normals of the second triangle
	if (!Triangle<T>::are_projections_collided(n2, *this, that))
	{
		return false;
	}
	
	// for (auto nside : n_ssides) {
	// 	if (!Triangle<T>::are_projections_collided(nside, *this, that)) {
	// 		return false;
	// 	}
	// }

	// All cross products of their sides
	for (auto fside : fsides) {
		for (auto sside : ssides) {
			Vector3<T> _temp = Vector3<T>::cross_product(fside, sside);
			if (!Triangle<T>::are_projections_collided(_temp, *this, that))
				return false;
		}
	}

	/*
	

	// Check each side of the triangles and their normal
	if (!Triangle<T>::are_projections_collided(n1, *this, that)) {
		return false;
	}
	for (auto nside : n_fsides) {
		if (!Triangle<T>::are_projections_collided(nside, *this, that)) {
			return false;
		}
	}

	if (!Triangle<T>::are_projections_collided(n2, *this, that)) {
		return false;
	}
	for (auto nside : n_ssides) {
		if (!Triangle<T>::are_projections_collided(nside, *this, that)) {
			return false;
		}
	}

	// Check all cross products
	Vector3<T> _temp;
	// Normals
	_temp = Vector3<T>::cross_product(n1, n2);
	if (!_temp.is_zero() && !Triangle<T>::are_projections_collided(
			_temp, *this, that)) {
		return false;
	}
	// With normals
	for (auto nside : n_fsides) {
		_temp = Vector3<T>::cross_product(n2, nside);
		if (!_temp.is_zero() && !Triangle<T>::are_projections_collided(
				_temp, *this, that)) {
			return false;
		}
	}
	for (auto nside : n_ssides) {
		_temp = Vector3<T>::cross_product(n1, nside);
		if (!_temp.is_zero() && !Triangle<T>::are_projections_collided(
				_temp, *this, that)) {
			return false;
		}
	}

	for (auto fnside : n_fsides) {
		for (auto snside : n_ssides) {
			_temp = Vector3<T>::cross_product(fnside, snside);
			if (!_temp.is_zero() && !Triangle<T>::are_projections_collided(
					_temp, *this, that)) {
				return false;
			}
		}
	}
	*/
	return true;
}

}
