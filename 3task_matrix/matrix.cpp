#include "matrix.h"
#include <gtest/gtest.h>

#include "tests.h"

using namespace mofn;

int main(int argc, char *argv[]) {
#ifdef GTESTS
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#endif

    int n = 0;

    std::cin >> n;

    Matrix<float> matrix{n, n, 0};
    std::cin >> matrix;
    std::cout << matrix.determinant() << std::endl;

    return 0;
}