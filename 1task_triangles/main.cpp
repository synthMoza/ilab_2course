#include <iostream>
#include <vector>

#include "Vector3.h"
#include "Triangle.h"

//! Create N triangles by taking point's coordinates
//! from the input stream
template <typename T>
std::vector<Triangle<T>> create_triangles(int N);

int main() {
	int N = 0;
	std::vector<Triangle<float>> triangles;

	std::cout << "Enter the amount of triangles: ";
	std::cin >> N;

	triangles = create_triangles<float>(N);

	for (int i = 0; i < N; i++) {
		for (int j = i; j< N; j++) {
			if (triangles.at(i).is_collided(triangles.at(j))) {
				std::cout << i << " ";
				break;
			}
		}
	}

	return 0;
}
template <typename T>
std::vector<Triangle<T>> create_triangles(int N) {
	std::vector<Triangle<T>> triangles;

	for (int i = 0; i < N; i++) {
		std::vector<Vector3<T>> points;

		std::cout << "Enter points of triangle " << i << ": ";

		for (int j = 0; j < 3; j++)
			std::cin >> points.at(j).x_ >>  points.at(j).y_ >> points.at(j).z_;

		Triangle<T> triangle(points);
		triangles.push_back(triangle);
	}

	return triangles;
}
