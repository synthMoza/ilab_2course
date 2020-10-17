#include <iostream>
#include <vector>
#include <list>

#include "Vector3.h"
#include "Triangle.h"
#include "OctoTree.h"
#include "tests.h"

#include <gtest/gtest.h>

using namespace mfn;

//! Create N triangles by taking point's coordinates
//! from the input stream
template<typename T>
std::list<Triangle<T>> create_triangles(int N);

int main(int argc, char *argv[]) {
#ifdef GTESTS
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#endif
	int N = 0;
	std::list<Triangle<float>> triangles;

	//std::cout << "Enter the amount of triangles: ";
	std::cin >> N;

	if (N <= 0) {
		printf("Not a valid amount of triangles!\n");
		return -1;
	}

	triangles = create_triangles<float>(N);
#ifdef N2
	for (auto it1 = triangles.begin(); it1 != triangles.end(); ++it1)
	{
		for (auto it2 = triangles.begin(); it2 != triangles.end(); ++it2)
		{
			if (it1 != it2)
			{
				if (it1->is_collided(*it2))
				{
					std::cout << it1->number << " ";
					break;
				}
			}
		}
	}

	std::cout << std::endl;
	return 0;
#endif
	OctoTree<float> tree(triangles);
	OctoNode<float> *head = tree.getHead();
	tree.generateTree(head);
	tree.collision(head);
	//tree.print(head);
	std::cout << std::endl;
#ifdef COLLISION_AMOUNT
	std::cout << "Collision tests: " << tree.k << std::endl;
#endif

	return 0;
}
template<typename T>
std::list<Triangle<T>> create_triangles(int N) {
	std::list<Triangle<T>> triangles;

	for (int i = 0; i < N; i++) {
		std::vector<Vector3<T>> points;
		points.resize(3);

		//std::cout << "Enter points of triangle " << i << ": ";

		for (int j = 0; j < 3; j++)
			std::cin >> points.at(j).x_ >> points.at(j).y_ >> points.at(j).z_;

		Triangle<T> triangle(points);
		triangle.number = i;
		triangles.push_back(triangle);
	}

	return triangles;
}
