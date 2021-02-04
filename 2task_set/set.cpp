#include "set.h"

namespace mfns {

// Insert a new value in the given tree
Node* insert(Node* p, int key) {
    if (p == nullptr)
        return new Node(key);

    if (key < p->key_)
        p->leftChild = insert(p->leftChild, key);
    else
        p->rightChild = insert(p->rightChild, key);

    return p->balanceTree();
}
}

using namespace mfns;

// Find all values from the given intervals using AVL tree
std::vector<int> avlset(std::vector<int> data, std::vector<std::pair<int, int>> requests) {
    Node* set = nullptr;
    std::vector<int> result;

    try {
        for (auto key : data)
            set = insert(set, key);

        int res = 0;
        for (auto request : requests) {
            res = set->distance(request.first, request.second);
            result.push_back(res);
        }
    } 
    catch (std::exception& exception) {
        std::cout << exception.what();
        throw ;
    }

    if (set != nullptr)
        delete set;

    return result;
}

// Find all values from the given intervals using std::set
std::vector<int> stdset(std::vector<int> data, std::vector<std::pair<int, int>> requests) {
    std::set<int> set;
    std::vector<int> result;

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

        result.push_back(res);
    }

    return result;
}