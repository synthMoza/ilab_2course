#pragma once

#include <gtest/gtest.h>
#include "Vector3.h"

TEST(SCALAR_PRODUCT, TEST_1) {
    Vector3<float> vec1 = {0, 0, 0};
    Vector3<float> vec2 = {0, 0, 0};

    float res = Vector3<float>::scalar_product(vec1, vec2);

    EXPECT_FLOAT_EQ(res, 0);
}

TEST(SCALAR_PRODUCT, TEST_2) {
    Vector3<float> vec1 = {2, -2, 4};
    Vector3<float> vec2 = {0, 3, 1};

    float res = Vector3<float>::scalar_product(vec1, vec2);

    EXPECT_FLOAT_EQ(res, -2);
}

TEST(SCALAR_PRODUCT, TEST_3)
{
    Vector3<float> vec1 = {1, 8, -3};
    Vector3<float> vec2 = {10, 1, 6};

    float res = Vector3<float>::scalar_product(vec1, vec2);

    EXPECT_FLOAT_EQ(res, 0);
}

TEST(SCALAR_PRODUCT, TEST_4)
{
    Vector3<float> vec1 = {4, -2, -3};
    Vector3<float> vec2 = {-2, 5, 4};

    float res = Vector3<float>::scalar_product(vec1, vec2);

    EXPECT_FLOAT_EQ(res, -30);
}

TEST(CROSS_PRODUCT, TEST_1) {
    Vector3<float> vec1 = {0, 0, 0};
    Vector3<float> vec2 = {0, 0, 0};

    Vector3<float> res = Vector3<float>::cross_product(vec1, vec2);

    EXPECT_FLOAT_EQ(res.x_, 0);
    EXPECT_FLOAT_EQ(res.y_, 0);
    EXPECT_FLOAT_EQ(res.z_, 0);
}

TEST(CROSS_PRODUCT, TEST_2)
{
    Vector3<float> vec1 = {1, 2, 0};
    Vector3<float> vec2 = {0, 3, 4};

    Vector3<float> res = Vector3<float>::cross_product(vec1, vec2);

    EXPECT_FLOAT_EQ(res.x_, 8);
    EXPECT_FLOAT_EQ(res.y_, -4);
    EXPECT_FLOAT_EQ(res.z_, 3);
}

TEST(CROSS_PRODUCT, TEST_3)
{
    Vector3<float> vec1 = {-2, 6, 4};
    Vector3<float> vec2 = {2, 4, -3};

    Vector3<float> res = Vector3<float>::cross_product(vec1, vec2);

    EXPECT_FLOAT_EQ(res.x_, -34);
    EXPECT_FLOAT_EQ(res.y_, 2);
    EXPECT_FLOAT_EQ(res.z_, -20);
}

TEST(CROSS_PRODUCT, TEST_4)
{
    Vector3<float> vec1 = {7, -2, -1};
    Vector3<float> vec2 = {-3, 3, 4};

    Vector3<float> res = Vector3<float>::cross_product(vec1, vec2);

    EXPECT_FLOAT_EQ(res.x_, -5);
    EXPECT_FLOAT_EQ(res.y_, -25);
    EXPECT_FLOAT_EQ(res.z_, 15);
}