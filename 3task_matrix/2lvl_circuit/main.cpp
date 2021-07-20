#include "matrix.h"

#include <iostream>

using namespace se;

Matrix<int> foo() {
	Matrix<int> temp(100, 100);
	return temp;
}

int main() {
	Matrix<int> A(100, 100);
	A = Matrix<int>(200, 200);
	
	return 0;
}