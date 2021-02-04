#include <iostream>
#include <vector>
#include <algorithm>

int test_number = 0;

// Generates an arithmetic progression of n elements with the given 
// common difference, random m requests
void test1(int d, int n, int m) {
    int data[n];
    int elem1 = 0, elem2 = 0;
    std::string path_test = std::string("Tests/test") + std::to_string(test_number) + ".txt";
    std::string ans_test = std::string("Tests/ans") + std::to_string(test_number) + ".txt";
    test_number++;
    FILE* test = fopen(path_test.c_str(), "w");
    FILE* ans = fopen(ans_test.c_str(), "w");
    
    fprintf(test, "%d ", n);
    for (int i = 0; i < n; ++i) {
        data[i] = i * d;
        fprintf(test, "%d ", data[i]);
    }
    fprintf(test, "%d ", m);
    srand((unsigned) time(0));
    for (int i = 0; i < m; ++i) {
        elem1 = (rand() % n) * d;
        elem2 = (rand() % n) * d;
        // elem1 <= elem2
        if (elem1 > elem2)
            std::swap(elem1, elem2);
        fprintf(test, "%d %d ", elem1, elem2);
        fprintf(ans, "%d ", (elem2 - elem1) / d + 1);
    }

    fclose(test);
    fclose(ans);
}

// Checks if the element is in the vector
template <typename T>
bool in_vector(std::vector<T> vector, T key) {
    for (auto elem : vector) {
        if (elem == key)
            return true;
    }

    return false;
}

// Random n number in the set, random m requests, max is the highest avaliable number
void test2(int n, int m, int max) {
    std::vector<int> data;
    int count = 0;
    int elem = 0;
    int elem1 = 0, elem2 = 0;
    std::string path_test = std::string("Tests/test") + std::to_string(test_number) + ".txt";
    std::string ans_test = std::string("Tests/ans") + std::to_string(test_number) + ".txt";
    test_number++;
    FILE* test = fopen(path_test.c_str(), "w");
    FILE* ans = fopen(ans_test.c_str(), "w");

    fprintf(test, "%d ", n);
    srand((unsigned) time(0));
    for (int i = 0; i < n; ++i) {
        elem = rand() % max;
        while (in_vector(data, elem)) {
            elem++;
        }
        data.push_back(elem);
        fprintf(test, "%d ", data.at(i));
    }

    fprintf(test, "%d ", m);
    std::sort(data.begin(), data.end());
    for (int i = 0; i < m; ++i) {
        elem1 = rand() % max;
        elem2 = rand() % max;
        // elem1 <= elem2
        if (elem1 > elem2)
            std::swap(elem1, elem2);
        fprintf(test, "%d %d ", elem1, elem2);
        count = 0;
        for (auto elem : data) {
            // Look for every elem in the vector
            if (elem >= elem1) {
                if (elem <= elem2)
                    count++;
                else
                    break;
            }
        }
        fprintf(ans, "%d ", count);
    }

    fclose(test);
    fclose(ans);
}

int main() {
    // test1(2, 10, 5);
    // test1(3, 5, 10);
    // test1(5, 20, 10);

    // test2(10, 5, 20);
    // test2(20, 40, 100);
    // test2(50, 50, 250);

    test1(192, 200, 5000);
    test1(57, 500, 10000);
    test1(37, 1000, 100000);

    test2(100, 5000, INT32_MAX);
    test2(1000, 10000, INT32_MAX);
    test2(15000, 100000, INT32_MAX);

    return 0;
}