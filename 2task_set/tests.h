#pragma once

#include <gtest/gtest.h>
#include "set.h"

TEST(AVLTREE, TEST0) {
    int n = 0, m = 0;
    int temp = 0;
    int req1 = 0, req2 = 0;
    std::vector<int> result;
    std::vector<int> answer;
    std::vector<int> data;
    std::vector<std::pair<int, int>> requests;

    std::string path_test = "Tests/test0.txt";
    std::string ans_test = "Tests/ans0.txt";
    FILE* ans = fopen(ans_test.c_str(), "r");
    FILE* test = fopen(path_test.c_str(), "r");

    fscanf(test, "%d", &n);
    for (int i = 0; i < n; ++i) {
        fscanf(test, "%d", &temp);
        data.push_back(temp);
    }

    fscanf(test, "%d", &m);
    for (int i = 0; i < m; ++i) {
        fscanf(test, "%d", &req1);
        fscanf(test, "%d", &req2);
        requests.push_back(std::pair<int, int>(req1,req2));
    }

    result = avlset(data.begin(), data.end(), requests.begin(), requests.end());
    for (int i = 0; i < m; ++i) {
        fscanf(ans, "%d", &temp);
        answer.push_back(temp);
    }    

    fclose(test);
    fclose(ans);

    EXPECT_EQ(result, answer);
}


TEST(AVLTREE, TEST1) {
    int n = 0, m = 0;
    int temp = 0;
    int req1 = 0, req2 = 0;
    std::vector<int> result;
    std::vector<int> answer;
    std::vector<int> data;
    std::vector<std::pair<int, int>> requests;

    std::string path_test = "Tests/test1.txt";
    std::string ans_test = "Tests/ans1.txt";
    FILE* ans = fopen(ans_test.c_str(), "r");
    FILE* test = fopen(path_test.c_str(), "r");

    fscanf(test, "%d", &n);
    for (int i = 0; i < n; ++i) {
        fscanf(test, "%d", &temp);
        data.push_back(temp);
    }

    fscanf(test, "%d", &m);
    for (int i = 0; i < m; ++i) {
        fscanf(test, "%d", &req1);
        fscanf(test, "%d", &req2);
        requests.push_back(std::pair<int, int>(req1,req2));
    }

    result = avlset(data.begin(), data.end(), requests.begin(), requests.end());
    for (int i = 0; i < m; ++i) {
        fscanf(ans, "%d", &temp);
        answer.push_back(temp);
    }    

    fclose(test);
    fclose(ans);

    EXPECT_EQ(result, answer);
}

TEST(AVLTREE, TEST2) {
    int n = 0, m = 0;
    int temp = 0;
    int req1 = 0, req2 = 0;
    std::vector<int> result;
    std::vector<int> answer;
    std::vector<int> data;
    std::vector<std::pair<int, int>> requests;

    std::string path_test = "Tests/test2.txt";
    std::string ans_test = "Tests/ans2.txt";
    FILE* ans = fopen(ans_test.c_str(), "r");
    FILE* test = fopen(path_test.c_str(), "r");

    fscanf(test, "%d", &n);
    for (int i = 0; i < n; ++i) {
        fscanf(test, "%d", &temp);
        data.push_back(temp);
    }

    fscanf(test, "%d", &m);
    for (int i = 0; i < m; ++i) {
        fscanf(test, "%d", &req1);
        fscanf(test, "%d", &req2);
        requests.push_back(std::pair<int, int>(req1,req2));
    }

    result = avlset(data.begin(), data.end(), requests.begin(), requests.end());
    for (int i = 0; i < m; ++i) {
        fscanf(ans, "%d", &temp);
        answer.push_back(temp);
    }    

    fclose(test);
    fclose(ans);

    EXPECT_EQ(result, answer);
}

TEST(AVLTREE, TEST3) {
    int n = 0, m = 0;
    int temp = 0;
    int req1 = 0, req2 = 0;
    std::vector<int> result;
    std::vector<int> answer;
    std::vector<int> data;
    std::vector<std::pair<int, int>> requests;

    std::string path_test = "Tests/test3.txt";
    std::string ans_test = "Tests/ans3.txt";
    FILE* ans = fopen(ans_test.c_str(), "r");
    FILE* test = fopen(path_test.c_str(), "r");

    fscanf(test, "%d", &n);
    for (int i = 0; i < n; ++i) {
        fscanf(test, "%d", &temp);
        data.push_back(temp);
    }

    fscanf(test, "%d", &m);
    for (int i = 0; i < m; ++i) {
        fscanf(test, "%d", &req1);
        fscanf(test, "%d", &req2);
        requests.push_back(std::pair<int, int>(req1,req2));
    }

    result = avlset(data.begin(), data.end(), requests.begin(), requests.end());
    for (int i = 0; i < m; ++i) {
        fscanf(ans, "%d", &temp);
        answer.push_back(temp);
    }    

    fclose(test);
    fclose(ans);

    EXPECT_EQ(result, answer);
}

TEST(AVLTREE, TEST4) {
    int n = 0, m = 0;
    int temp = 0;
    int req1 = 0, req2 = 0;
    std::vector<int> result;
    std::vector<int> answer;
    std::vector<int> data;
    std::vector<std::pair<int, int>> requests;

    std::string path_test = "Tests/test4.txt";
    std::string ans_test = "Tests/ans4.txt";
    FILE* ans = fopen(ans_test.c_str(), "r");
    FILE* test = fopen(path_test.c_str(), "r");

    fscanf(test, "%d", &n);
    for (int i = 0; i < n; ++i) {
        fscanf(test, "%d", &temp);
        data.push_back(temp);
    }

    fscanf(test, "%d", &m);
    for (int i = 0; i < m; ++i) {
        fscanf(test, "%d", &req1);
        fscanf(test, "%d", &req2);
        requests.push_back(std::pair<int, int>(req1,req2));
    }

    result = avlset(data.begin(), data.end(), requests.begin(), requests.end());
    for (int i = 0; i < m; ++i) {
        fscanf(ans, "%d", &temp);
        answer.push_back(temp);
    }    

    fclose(test);
    fclose(ans);

    EXPECT_EQ(result, answer);
}

TEST(AVLTREE, TEST5) {
    int n = 0, m = 0;
    int temp = 0;
    int req1 = 0, req2 = 0;
    std::vector<int> result;
    std::vector<int> answer;
    std::vector<int> data;
    std::vector<std::pair<int, int>> requests;

    std::string path_test = "Tests/test5.txt";
    std::string ans_test = "Tests/ans5.txt";
    FILE* ans = fopen(ans_test.c_str(), "r");
    FILE* test = fopen(path_test.c_str(), "r");

    fscanf(test, "%d", &n);
    for (int i = 0; i < n; ++i) {
        fscanf(test, "%d", &temp);
        data.push_back(temp);
    }

    fscanf(test, "%d", &m);
    for (int i = 0; i < m; ++i) {
        fscanf(test, "%d", &req1);
        fscanf(test, "%d", &req2);
        requests.push_back(std::pair<int, int>(req1,req2));
    }

    result = avlset(data.begin(), data.end(), requests.begin(), requests.end());
    for (int i = 0; i < m; ++i) {
        fscanf(ans, "%d", &temp);
        answer.push_back(temp);
    }    

    fclose(test);
    fclose(ans);

    EXPECT_EQ(result, answer);
}

TEST(STDSET, TEST0) {
    int n = 0, m = 0;
    int temp = 0;
    int req1 = 0, req2 = 0;
    std::vector<int> result;
    std::vector<int> answer;
    std::vector<int> data;
    std::vector<std::pair<int, int>> requests;

    std::string path_test = "Tests/test0.txt";
    std::string ans_test = "Tests/ans0.txt";
    FILE* ans = fopen(ans_test.c_str(), "r");
    FILE* test = fopen(path_test.c_str(), "r");

    fscanf(test, "%d", &n);
    for (int i = 0; i < n; ++i) {
        fscanf(test, "%d", &temp);
        data.push_back(temp);
    }

    fscanf(test, "%d", &m);
    for (int i = 0; i < m; ++i) {
        fscanf(test, "%d", &req1);
        fscanf(test, "%d", &req2);
        requests.push_back(std::pair<int, int>(req1,req2));
    }

    result = stdset(data.begin(), data.end(), requests.begin(), requests.end());
    for (int i = 0; i < m; ++i) {
        fscanf(ans, "%d", &temp);
        answer.push_back(temp);
    }    

    fclose(test);
    fclose(ans);

    EXPECT_EQ(result, answer);
}


TEST(STDSET, TEST1) {
    int n = 0, m = 0;
    int temp = 0;
    int req1 = 0, req2 = 0;
    std::vector<int> result;
    std::vector<int> answer;
    std::vector<int> data;
    std::vector<std::pair<int, int>> requests;

    std::string path_test = "Tests/test1.txt";
    std::string ans_test = "Tests/ans1.txt";
    FILE* ans = fopen(ans_test.c_str(), "r");
    FILE* test = fopen(path_test.c_str(), "r");

    fscanf(test, "%d", &n);
    for (int i = 0; i < n; ++i) {
        fscanf(test, "%d", &temp);
        data.push_back(temp);
    }

    fscanf(test, "%d", &m);
    for (int i = 0; i < m; ++i) {
        fscanf(test, "%d", &req1);
        fscanf(test, "%d", &req2);
        requests.push_back(std::pair<int, int>(req1,req2));
    }

    result = stdset(data.begin(), data.end(), requests.begin(), requests.end());
    for (int i = 0; i < m; ++i) {
        fscanf(ans, "%d", &temp);
        answer.push_back(temp);
    }    

    fclose(test);
    fclose(ans);

    EXPECT_EQ(result, answer);
}

TEST(STDSET, TEST2) {
    int n = 0, m = 0;
    int temp = 0;
    int req1 = 0, req2 = 0;
    std::vector<int> result;
    std::vector<int> answer;
    std::vector<int> data;
    std::vector<std::pair<int, int>> requests;

    std::string path_test = "Tests/test2.txt";
    std::string ans_test = "Tests/ans2.txt";
    FILE* ans = fopen(ans_test.c_str(), "r");
    FILE* test = fopen(path_test.c_str(), "r");

    fscanf(test, "%d", &n);
    for (int i = 0; i < n; ++i) {
        fscanf(test, "%d", &temp);
        data.push_back(temp);
    }

    fscanf(test, "%d", &m);
    for (int i = 0; i < m; ++i) {
        fscanf(test, "%d", &req1);
        fscanf(test, "%d", &req2);
        requests.push_back(std::pair<int, int>(req1,req2));
    }

    result = stdset(data.begin(), data.end(), requests.begin(), requests.end());
    for (int i = 0; i < m; ++i) {
        fscanf(ans, "%d", &temp);
        answer.push_back(temp);
    }    

    fclose(test);
    fclose(ans);

    EXPECT_EQ(result, answer);
}

TEST(STDSET, TEST3) {
    int n = 0, m = 0;
    int temp = 0;
    int req1 = 0, req2 = 0;
    std::vector<int> result;
    std::vector<int> answer;
    std::vector<int> data;
    std::vector<std::pair<int, int>> requests;

    std::string path_test = "Tests/test3.txt";
    std::string ans_test = "Tests/ans3.txt";
    FILE* ans = fopen(ans_test.c_str(), "r");
    FILE* test = fopen(path_test.c_str(), "r");

    fscanf(test, "%d", &n);
    for (int i = 0; i < n; ++i) {
        fscanf(test, "%d", &temp);
        data.push_back(temp);
    }

    fscanf(test, "%d", &m);
    for (int i = 0; i < m; ++i) {
        fscanf(test, "%d", &req1);
        fscanf(test, "%d", &req2);
        requests.push_back(std::pair<int, int>(req1,req2));
    }

    result = stdset(data.begin(), data.end(), requests.begin(), requests.end());
    for (int i = 0; i < m; ++i) {
        fscanf(ans, "%d", &temp);
        answer.push_back(temp);
    }    

    fclose(test);
    fclose(ans);

    EXPECT_EQ(result, answer);
}

TEST(STDSET, TEST4) {
    int n = 0, m = 0;
    int temp = 0;
    int req1 = 0, req2 = 0;
    std::vector<int> result;
    std::vector<int> answer;
    std::vector<int> data;
    std::vector<std::pair<int, int>> requests;

    std::string path_test = "Tests/test4.txt";
    std::string ans_test = "Tests/ans4.txt";
    FILE* ans = fopen(ans_test.c_str(), "r");
    FILE* test = fopen(path_test.c_str(), "r");

    fscanf(test, "%d", &n);
    for (int i = 0; i < n; ++i) {
        fscanf(test, "%d", &temp);
        data.push_back(temp);
    }

    fscanf(test, "%d", &m);
    for (int i = 0; i < m; ++i) {
        fscanf(test, "%d", &req1);
        fscanf(test, "%d", &req2);
        requests.push_back(std::pair<int, int>(req1,req2));
    }

    result = stdset(data.begin(), data.end(), requests.begin(), requests.end());
    for (int i = 0; i < m; ++i) {
        fscanf(ans, "%d", &temp);
        answer.push_back(temp);
    }    

    fclose(test);
    fclose(ans);

    EXPECT_EQ(result, answer);
}

TEST(STDSET, TEST5) {
    int n = 0, m = 0;
    int temp = 0;
    int req1 = 0, req2 = 0;
    std::vector<int> result;
    std::vector<int> answer;
    std::vector<int> data;
    std::vector<std::pair<int, int>> requests;

    std::string path_test = "Tests/test5.txt";
    std::string ans_test = "Tests/ans5.txt";
    FILE* ans = fopen(ans_test.c_str(), "r");
    FILE* test = fopen(path_test.c_str(), "r");

    fscanf(test, "%d", &n);
    for (int i = 0; i < n; ++i) {
        fscanf(test, "%d", &temp);
        data.push_back(temp);
    }

    fscanf(test, "%d", &m);
    for (int i = 0; i < m; ++i) {
        fscanf(test, "%d", &req1);
        fscanf(test, "%d", &req2);
        requests.push_back(std::pair<int, int>(req1,req2));
    }

    result = stdset(data.begin(), data.end(), requests.begin(), requests.end());
    for (int i = 0; i < m; ++i) {
        fscanf(ans, "%d", &temp);
        answer.push_back(temp);
    }    

    fclose(test);
    fclose(ans);

    EXPECT_EQ(result, answer);
}