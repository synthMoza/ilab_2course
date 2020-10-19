#include "matrix.h"

using namespace mofn;

int main() {
    Matrix<int> A{5, 5, 4};
    Matrix<int> B = -A;

    A.print();
    B.print();

    return 0;
}