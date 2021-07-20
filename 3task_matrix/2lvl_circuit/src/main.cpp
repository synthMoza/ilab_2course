#include "matrix.h"

#include <iostream>

using namespace se;

int main() {
	try {
		Matrix<float> A(3, 3);

		std::cin >> A;
		A ^= 2;
		std::cout << A;
	} catch (std::exception& e) {
		std::cerr << "Caught exception: ";
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}