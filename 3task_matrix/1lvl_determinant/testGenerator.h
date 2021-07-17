#pragma once

#include <iostream>

namespace mofn {
    int test_n = 1;

    #define MATRIX_RAND_MAX 20;

    //! Generate a diagonal matrix "n * n" with random number between 1 and MATRIX_RAND_MAX
    //! As the determinant is the product of n numbers (~ (MATRIX_RAND_MAX / 2)^n) and the
    //! determinant is "int" (Matrix<int>), n must be about log_{INT_MAX}(MATRIX_RAND_MAX / 2) 
    void diagTest(int n);
    //! Generates diagonal matrix, but rotates it at the given angle without changing the determinant
    void volumeTest(int n, float ang);
}