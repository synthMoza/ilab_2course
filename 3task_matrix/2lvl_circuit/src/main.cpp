#include <iostream>
#include <utility>

#ifdef DEBUG
#include <gtest/gtest.h>
#include "tests/tests.h"
#else
#include "circuit.h"
#include "driver.h"
#include "matrix.h"

using namespace se;
#endif

int main(int argc, char* argv[]) {
#ifdef DEBUG
    ::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#else
	yy::Driver driver;
    if (!driver.parse()) {
        std::cerr << "Failed to parse the input code!" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
#endif
}
