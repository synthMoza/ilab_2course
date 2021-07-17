#include "matrix.h"
#include "testGenerator.h"
#include <vector>

using namespace mofn;

int main() {
    diagTest(10);
    diagTest(5);
    volumeTest(3, 2.f);
    volumeTest(5, 1.2);

    return 0;
}

namespace mofn {

void diagTest(int n) {
    int number = 0;
    int result = 1;
    std::string fname = "Tests/matrix_test" + std::to_string(test_n) + ".txt"; 
    std::string aname = "Tests/matrix_ans" + std::to_string(test_n) + ".txt";
    test_n++;

    FILE* file = fopen(fname.c_str(), "w");
    FILE* ans = fopen(aname.c_str(), "w");
    fprintf(file, "%d \n", n);

    for (int i = 0; i < n; i++) {
        number = rand() % MATRIX_RAND_MAX + 1;
        result *= number;

        for (int j = 0; j < i; j++) {
            fprintf(file, "%d ", 0);
            //printf("%d ", 0);
        }

        fprintf(file, "%d ", number);
        //printf("%d ", number);

        for (int j = i + 1; j < n; j++) {
            fprintf(file, "%d ", 0);
            //printf("%d ", 0);
        }

        //fprintf(file, "\n");
        //printf("\n");
    }

    fprintf(ans, "%d\n", result);

    fclose(file);
    fclose(ans);
}

void volumeTest(int n, float ang) {
    Matrix<float> rot = rotation(n, 0, 1, ang);
    int answer = 1;
    int rnd = 0;
    std::vector<Matrix<float>> vectors{n, Matrix<float>{n, 1, 0.f}};

    std::string fname = "Tests/matrix_test" + std::to_string(test_n) + ".txt";
    std::string aname = "Tests/matrix_ans" + std::to_string(test_n) + ".txt";
    test_n++;

    FILE *file = fopen(fname.c_str(), "w");
    FILE *ans = fopen(aname.c_str(), "w");

    fprintf(file, "%d \n", n);
    for (int i = 0; i < n; ++i) {
        rnd = rand() % MATRIX_RAND_MAX + 1;
        answer *= rnd;
        vectors[i][i][0] = rnd;
        vectors[i] = rot * vectors[i]; // rotate vectors without changing the volume
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            fprintf(file, "%f ", vectors[j][i][0]);
        }
    }

    fprintf(ans, "%d", answer);

    fclose(ans);
    fclose(file);
}
}