#pragma once

#include "Vector3.h"
#include <vector>

template <typename T>
class Triangle {
	std::vector<Vector3<T>> points_;
public:
	Triangle(const std::vector<Vector3<T>> &points);

	bool is_collided(const Triangle& that);
};

template<typename T>
inline Triangle<T>::Triangle(const std::vector<Vector3<T> > &points) {
	// Copy input vector
	points_ = points;
}

template<typename T>
inline bool Triangle<T>::is_collided(const Triangle &that) {


}
