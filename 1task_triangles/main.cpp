#include <iostream>
#include <vector>

//! Create N triangles by taking point's coordinates
//! from the input stream
template <typename T>
std::vector<T> create_triangles(int N);

int main() {
	int N = 0;
	std::vector<float> triangles;

	std::cout << "Enter the amount of points: ";
	std::cin >> N;

	triangles = create_triangles<float>(N);

	return 0;
}
template <typename T>
std::vector<T> create_triangles(int N) {

}
