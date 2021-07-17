#include "set.h"

int main() {
    mfns::avlset<int> set{1, 2, 5};
    mfns::avlset<int> tmp;
    tmp = set;
    tmp.print();

    return EXIT_SUCCESS;
}