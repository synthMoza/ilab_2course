#include "set.h"

// All functiosn declaration
void stdset(std::vector<int> data, std::vector<std::pair<int, int>> requests);
void avlset(std::vector<int> data, std::vector<std::pair<int, int>> requests);

int main() {
    int n = 0;
    int m = 0;
    int input = 0;
    std::pair<int, int> request;
    std::vector<int> data;
    std::vector<std::pair<int, int>> requests;

    // Input set's data
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> input;
        data.push_back(input);
    }

    // Input requests
    std::cin >> m;
    for (int i = 0 ; i < m; ++i) {
        std::cin >> request.first >> request.second;
        requests.push_back(request);
    }

    //=================================================
    auto start = std::chrono::system_clock::now();
    stdset(data, requests);
    auto end = std::chrono::system_clock::now();
    //=================================================
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "STDSET, elapsed time: " << elapsed.count() << "s" << std::endl;

    //=================================================
    start = std::chrono::system_clock::now();
    avlset(data, requests);
    end = std::chrono::system_clock::now();
    //=================================================

    elapsed = end - start;
    std::cout << "AVLSET, elapsed time: " << elapsed.count() << "s" << std::endl;

    return EXIT_SUCCESS;
}

void stdset(std::vector<int> data, std::vector<std::pair<int, int>> requests) {
    std::set<int> set;

    for (auto elem : data)
        set.insert(elem);

    std::set<int>::iterator it1, it2;
    int res = 0;

    for (auto request : requests) {

        it1 = set.lower_bound(request.first);
        it2 = set.lower_bound(request.second);
        res = std::distance(it1, it2);
        if (it2 == set.find(request.second) && it2 != set.end())
            res++;

        std::cout << res << " ";
    }

    std::cout << std::endl;
}

using namespace mfns;

void avlset(std::vector<int> data, std::vector<std::pair<int, int>> requests) {
    Node* set = nullptr;

    for (auto key : data)
        set = insert(set, key);

    int res = 0;
    for (auto request : requests) {
        res = distance(set, request.first, request.second);

        std::cout << res << " ";
    }

    std::cout << std::endl;
}