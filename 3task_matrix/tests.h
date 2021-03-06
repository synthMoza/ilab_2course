#pragma once

#include <iostream>
#include <gtest/gtest.h>
#include "matrix.h"

using namespace mofn;

TEST(DETERMINANT, TEST_1) {
    FILE* file = fopen("Tests/matrix_test1.txt", "r");
    FILE *ans = fopen("Tests/matrix_ans1.txt", "r");

    if (file == NULL || ans == NULL) {
        FAIL();
    }

    int n = 0;
    
    int answer = 0;
    int result = 0;

    fscanf(file, "%d", &n);
    fscanf(ans, "%d", &answer);

    mofn::Matrix<int> matrix{n, n, 0};

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            fscanf(file, "%d", &(matrix[i][j]));
        }

    result = matrix.determinant();
    fclose(file);
    fclose(ans);

    EXPECT_FLOAT_EQ(result, answer);
}

TEST(DETERMINANT, TEST_2) {
    FILE *file = fopen("Tests/matrix_test2.txt", "r");
    FILE *ans = fopen("Tests/matrix_ans2.txt", "r");

    if (file == NULL || ans == NULL)
    {
        FAIL();
    }

    int n = 0;

    int answer = 0;
    int result = 0;

    fscanf(file, "%d", &n);
    fscanf(ans, "%d", &answer);

    Matrix<int> matrix{n, n, 0};

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            fscanf(file, "%d", &(matrix[i][j]));
        }

    result = matrix.determinant();
    fclose(file);
    fclose(ans);

    EXPECT_FLOAT_EQ(result, answer);
}

TEST(DETERMINANT, TEST_3) {
    FILE *file = fopen("Tests/matrix_test3.txt", "r");
    FILE *ans = fopen("Tests/matrix_ans3.txt", "r");

    if (file == NULL || ans == NULL)
    {
        FAIL();
    }

    int n = 0;

    int answer = 0;
    int result = 0;

    fscanf(file, "%d", &n);
    fscanf(ans, "%d", &answer);

    Matrix<float> matrix{n, n, 0};

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            fscanf(file, "%f", &(matrix[i][j]));
        }

    result = matrix.determinant();
    fclose(file);
    fclose(ans);

    EXPECT_FLOAT_EQ(result, answer);
}

TEST(DETERMINANT, TEST_4) {
    FILE *file = fopen("Tests/matrix_test4.txt", "r");
    FILE *ans = fopen("Tests/matrix_ans4.txt", "r");

    if (file == NULL || ans == NULL)
    {
        FAIL();
    }

    int n = 0;

    int answer = 0;
    float result = 0;

    fscanf(file, "%d", &n);
    fscanf(ans, "%d", &answer);

    Matrix<float> matrix{n, n, 0};

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            fscanf(file, "%g", &(matrix[i][j]));
        }

    result = matrix.determinant();
    fclose(file);
    fclose(ans);

    EXPECT_FLOAT_EQ(result, answer);
}

TEST(DETERMINANT, TEST_5) {
    FILE *file = fopen("Tests/matrix_test5.txt", "r");
    FILE *ans = fopen("Tests/matrix_ans5.txt", "r");

    if (file == NULL || ans == NULL)
    {
        FAIL();
    }

    int n = 0;

    int answer = 0;
    int result = 0;

    fscanf(file, "%d", &n);
    fscanf(ans, "%d", &answer);

    Matrix<float> matrix{n, n, 0};

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            fscanf(file, "%f", &(matrix[i][j]));
        }

    result = matrix.determinant();
    fclose(file);
    fclose(ans);

    EXPECT_FLOAT_EQ(result, answer);
}

TEST(DETERMINANT, TEST_6) {
    FILE *file = fopen("Tests/matrix_test6.txt", "r");
    FILE *ans = fopen("Tests/matrix_ans6.txt", "r");

    if (file == NULL || ans == NULL)
    {
        FAIL();
    }

    int n = 0;

    int answer = 0;
    int result = 0;

    fscanf(file, "%d", &n);
    fscanf(ans, "%d", &answer);

    Matrix<float> matrix{n, n, 0};

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            fscanf(file, "%f", &(matrix[i][j]));
        }

    result = matrix.determinant();
    fclose(file);
    fclose(ans);

    EXPECT_FLOAT_EQ(result, answer);
}

TEST(DETERMINANT, TEST_7) {
    FILE *file = fopen("Tests/matrix_test7.txt", "r");
    FILE *ans = fopen("Tests/matrix_ans7.txt", "r");

    if (file == NULL || ans == NULL)
    {
        FAIL();
    }

    int n = 0;

    int answer = 0;
    int result = 0;

    fscanf(file, "%d", &n);
    fscanf(ans, "%d", &answer);

    Matrix<float> matrix{n, n, 0};

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            fscanf(file, "%f", &(matrix[i][j]));
        }

    result = matrix.determinant();
    fclose(file);
    fclose(ans);

    EXPECT_FLOAT_EQ(result, answer);
}

// TEST(DETERMINANT, TEST_8) {
//     Matrix<int> matrix = Matrix<int>::eye(1000);
//     int result = matrix.determinant();

//     EXPECT_EQ(result, 1);
// }

TEST(CHAINED_OPS, PLUS_1) {
    Matrix<int> A = {3, 3, 5};
    Matrix<int> B = {3, 3, 8};
    Matrix<int> ans = {3, 3, 13};

    A += B;

    EXPECT_EQ(A, ans);
}

TEST(CHAINED_OPS, PLUS_2)
{
    Matrix<int> A = {3, 6, 1};
    Matrix<int> B = {9, 2, 7};

    EXPECT_DEATH( { A += B; }, "");
}

TEST(CHAINED_OPS, MINUS_1)
{
    Matrix<int> A = {2, 2, 3};
    Matrix<int> B = {2, 2, 5};
    Matrix<int> ans = {2, 2, -2};

    A -= B;

    EXPECT_EQ(A, ans);
}

TEST(CHAINED_OPS, MINUS_2)
{
    Matrix<int> A = {3, 6, 1};
    Matrix<int> B = {9, 2, 7};

    EXPECT_DEATH({ A -= B; }, "");
}

TEST(CHAINED_OPS, MULTIPLY_1) {
    Matrix<int> A = {2, 5, 2};
    int n = 3;
    Matrix<int> ans = {2, 5, 2 * n};

    A *= n;

    EXPECT_EQ(A, ans);
}

TEST(CHAINED_OPS, MULTIPLY_2) {
    Matrix<int> A = {2, 2, 0};
    A[0][0] = 1;
    A[0][1] = -3;
    A[1][1] = 2;

    Matrix<int> B = {2, 2, 0};
    B[0][0] = 5;
    B[1][0] = 2;
    B[1][1] = 1;

    Matrix<int> ans = {2, 2, 0};
    ans[0][0] = -1;
    ans[0][1] = -3;
    ans[1][0] = 4;
    ans[1][1] = 2;

    A *= B;

    EXPECT_EQ(A, ans);
}

TEST(CHAINED_OPS, MULTIPLY_3) {
    Matrix<int> A{3, 2, 0};
    Matrix<int> B{5, 4, 0};

    EXPECT_DEATH({A *= B;}, "");
}

TEST(ORDINARY_OPS, PLUS_1) {
    Matrix<int> A = {3, 3, 5};
    Matrix<int> B = {3, 3, 8};
    Matrix<int> C{3, 3, 0};
    C = A + B;

    Matrix<int> ans = {3, 3, 13};

    EXPECT_EQ(C, ans);
}

TEST(ORDINARY_OPS, PLUS_2) {
    Matrix<int> A = {1, 7, 2};
    Matrix<int> B = {2, 9, 9};
    Matrix<int> C{3, 3, 0};
    EXPECT_DEATH({ C = A + B; }, "");
}

TEST(ORDINARY_OPS, MINUS_1) {
    Matrix<int> A = {3, 3, 5};
    Matrix<int> B = {3, 3, 8};
    Matrix<int> C{3, 3, 0};
    EXPECT_NO_THROW({ C = A - B; });

    Matrix<int> ans = {3, 3, -3};

    EXPECT_EQ(C, ans);
}

TEST(ORDINARY_OPS, MINUS_2) {
    Matrix<int> A = {1, 7, 2};
    Matrix<int> B = {2, 9, 9};
    Matrix<int> C{3, 3, 0};
    EXPECT_DEATH({ C = A - B; }, "");
}

TEST(ORDINARY_OPS, MULTIPLY_1) {
    Matrix<int> A = {2, 5, 2};
    Matrix<int> C = {2, 5, 0};
    int n = 3;
    Matrix<int> ans = {2, 5, 2 * n};

    C = A * n;

    EXPECT_EQ(C, ans);
}

TEST(ORDINARY_OPS, MULTIPLY_2) {
    Matrix<int> A = {2, 2, 0};
    A[0][0] = 1;
    A[0][1] = -3;
    A[1][1] = 2;

    Matrix<int> B = {2, 2, 0};
    B[0][0] = 5;
    B[1][0] = 2;
    B[1][1] = 1;
    Matrix<int> C = {2, 2, 0};

    Matrix<int> ans = {2, 2, 0};
    ans[0][0] = -1;
    ans[0][1] = -3;
    ans[1][0] = 4;
    ans[1][1] = 2;

    C = A * B;

    EXPECT_EQ(C, ans);
}

TEST(ORDINARY_OPS, MULTIPLY_3) {
    Matrix<int> A{3, 2, 0};
    Matrix<int> B{5, 4, 0};
    Matrix<int> C{3, 4, 0};

    EXPECT_DEATH({ C = A * B; }, "");
}

TEST(CHAINED_OPS, POW_1) {
    Matrix<int> A = {2, 2, 0};
    A[0][0] = 1;
    A[0][1] = -3;
    A[1][0] = 5;
    A[1][1] = 2;

    Matrix<int> ans = {2, 2, 0};
    ans[0][0] = -14;
    ans[0][1] = -9;
    ans[1][0] = 15;
    ans[1][1] = -11;

    A ^= 2;
    EXPECT_EQ(ans, A);
}

TEST(ORDINARY_OPS, POW_1) {
    Matrix<int> A = {2, 2, 0};
    A[0][0] = 1;
    A[0][1] = -3;
    A[1][0] = 5;
    A[1][1] = 2;

    Matrix<int> ans = {2, 2, 0};
    ans[0][0] = -14;
    ans[0][1] = -9;
    ans[1][0] = 15;
    ans[1][1] = -11;

    Matrix<int> res = {2, 2, 0};
    res = A ^ 2;
    EXPECT_EQ(ans, res);
}

TEST(ORDINARY_OPS, POW_2) {
    Matrix<int> A = {2, 2, 0};
    A[0][0] = 1;
    A[0][1] = -3;
    A[1][0] = 5;
    A[1][1] = 2;

    Matrix<int> ans = {2, 2, 0};
    ans[0][0] = 61;
    ans[0][1] = 225;
    ans[1][0] = -375;
    ans[1][1] = -14;

    Matrix<int> res = {2, 2, 0};
    res = A ^ 4;
    EXPECT_EQ(ans, res);
}

TEST(MINOR_TEST, MINOR_1) {
    Matrix<int> A = {3, 3, 3};
    A[0][0] = 4;
    int ans = 3;
    int res = A.getMinor(1, 1);

    EXPECT_EQ(res, ans);
}

TEST(MINOR_TEST, MINOR_2) {
    Matrix<int> A = {3, 3, -3};
    A[0][0] = 4;
    A[0][2] = 2;
    A[2][0] = 1;
    A[2][2] = 3;
    int ans = 10;
    int res = A.getMinor(1, 1);

    EXPECT_EQ(res, ans);
}