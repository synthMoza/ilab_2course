#include <iostream>

#include "unitTests.h"

int main() {
	unit_test_1(5, 30, 50);
	unit_test_1(500, 10000, 100000);

	unit_test_2(25, 500, 100, 10, 1000);
	unit_test_2(50, 2000, 300, 10, 10000);

	unit_test_3(25, 500, 100000);
	unit_test_3(30, 600, 500000);

	return 0;
}
