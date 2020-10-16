#include "Vector3.h"

#include <cmath>
#include <cstdlib>
#include <vector>

#define TEST_EXT ".tst"
#define ANS_EXT ".ans"

void test1(float length, int num_tr, int test_num);
void test2(float length, int num_tr, int test_num);

int main() {
    test1(5, 100, 1);
    test2(5, 100, 2);
    return 0;
}

// Generates num_tr triangles of given length placed in the column (no collisions)
void test1(float length, int num_tr, int test_num) {
    float dist = 0.f;
    float new_dist = 0.f;
    float height = 0.5 * sqrt(3) * length;

    float x1 = -0.5 * length;
    float x2 = 0.5 * length;

    std::string file_name = "Tests/" + std::to_string(num_tr) + "." + std::to_string(test_num);
    std::string test_name = file_name + TEST_EXT;
    std::string ans_name = file_name + ANS_EXT;

    FILE* tfile = fopen(test_name.c_str(), "w");
    FILE* afile = fopen(ans_name.c_str(), "w");
    fprintf(tfile, "%d\n", num_tr);

    // First triangle
    fprintf(tfile, "%g %g %g\n", x1, 0.f, 0.f);
    fprintf(tfile, "%g %g %g\n", 0.f, height, 0.f);
    fprintf(tfile, "%g %g %g\n", x2, 0.f, 0.f);
    fprintf(tfile, "\n");

    for (int i = 1; i < num_tr; i++) {
        new_dist = 2 * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        dist += new_dist;

        fprintf(tfile, "%g %g %g\n", x1, 0.f, dist);
        fprintf(tfile, "%g %g %g\n", 0.f, height, dist);
        fprintf(tfile, "%g %g %g\n", x2, 0.f, dist);
        fprintf(tfile, "\n");
    }

    fclose(afile);
    fclose(tfile);
}

// Generates pairs of triangles in different planes
void test2(float length, int num_tr, int test_num) {
    std::vector<int> answers;

    float eps = 1E-04;
    float dz = 0.f;
    float dist = 0.f;
    float new_dist = 0.f;
    float height = 0.5 * sqrt(3) * length;

    float x1 = -0.5 * length;
    float x2 = 0.5 * length;

    std::string file_name = "Tests/" + std::to_string(num_tr) + "." + std::to_string(test_num);
    std::string test_name = file_name + TEST_EXT;
    std::string ans_name = file_name + ANS_EXT;

    FILE *tfile = fopen(test_name.c_str(), "w");
    FILE *afile = fopen(ans_name.c_str(), "w");
    fprintf(tfile, "%d\n", num_tr);

    for (int i = 0; i < num_tr; i += 2) {
        fprintf(tfile, "%g %g %g\n", x1, 0.f, dz);
        fprintf(tfile, "%g %g %g\n", 0.f, height, dz);
        fprintf(tfile, "%g %g %g\n", x2, 0.f, dz);
        fprintf(tfile, "\n");

        dist = 2 * height * static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

        if (dist < height - eps) {
            answers.push_back(i);
            answers.push_back(i + 1);
        }

        fprintf(tfile, "%g %g %g\n", x1, dist, dz);
        fprintf(tfile, "%g %g %g\n", 0.f, dist + height, dz);
        fprintf(tfile, "%g %g %g\n", x2, dist, dz);
        fprintf(tfile, "\n");

        dz += 0.5;
    }

    for (auto ans : answers) {
        fprintf(afile, "%d ", ans);
    }
}