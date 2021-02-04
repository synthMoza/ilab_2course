#include "set.h"

namespace mfns {
// Balances the given tree (AVL)
Node* balanceTree(Node* p) {
    if (p == nullptr)
        throw std::runtime_error("Unexpected nullptr in " + std::string(__PRETTY_FUNCTION__));
    
    p->recalcHeight();
    if (p->bFactor() == 2) {
        if (p->rightChild->bFactor() < 0)
            p->rightChild = rightRotate(p->rightChild);
        return leftRotate(p);
    }    

    if (p->bFactor() == -2) {
        if (p->leftChild->bFactor() > 0)
            p->leftChild = leftRotate(p->leftChild);
        return rightRotate(p);
    }

    return p;
}

// Finds the lowest common ancestor
Node* lowestCA(Node* head, Node* p, Node* q) {
    if (p == nullptr || q == nullptr || head == nullptr)
        throw std::runtime_error("Unexpected nullptr in " + std::string(__PRETTY_FUNCTION__));

    int n1 = p->key_;
    int n2 = q->key_;
    int n = head->key_;

    if (n1 > n && n2 > n) {
        return lowestCA(head->rightChild, p, q);
    } else if (n1 < n && n2 < n) {
        return lowestCA(head->leftChild, p, q);
    } else 
        return head;
}

// Insert a new value in the given tree
Node* insert(Node* p, int key) {
    if (p == nullptr)
        return new Node(key);

    if (key < p->key_)
        p->leftChild = insert(p->leftChild, key);
    else
        p->rightChild = insert(p->rightChild, key);

    return balanceTree(p);
}

// Range-query (all values in this range), [p;q]
int distance(Node *head, int p, int q) {
    if (head == nullptr)
        throw std::runtime_error("Unexpected nullptr in " + std::string(__PRETTY_FUNCTION__));

    int res = 0;

    if (head->key_ >= p && head->key_ <= q) {
        // This key is in the interval
        res++;
        if (head->leftChild)
            res += distance(head->leftChild, p, q);
        if (head->rightChild)
            res += distance(head->rightChild, p, q);
    } else if (head->key_ < p) {
        if (head->rightChild)
            res += distance(head->rightChild, p, q);
    } else {
        // head->key_ > q
        if (head->leftChild)
            res += distance(head->leftChild, p, q);
    }

    return res;
}

// Simple right rotation of the given node
Node* rightRotate(Node* p) {
    if (p == nullptr || p->leftChild == nullptr)
        throw std::runtime_error("Unexpected nullptr in " + std::string(__PRETTY_FUNCTION__));

    Node* q = p->leftChild;    
    p->leftChild = q->rightChild;
    q->rightChild = p;

    p->recalcHeight();  
    q->recalcHeight();

    return q;
}

// Simple left rotation of the given node
Node* leftRotate(Node *q) {
    if (q == nullptr || q->rightChild == nullptr)
        throw std::runtime_error("Unexpected nullptr in " + std::string(__PRETTY_FUNCTION__));
    
    Node *p = q->rightChild;
    q->rightChild = p->leftChild;
    p->leftChild = q;

    q->recalcHeight();
    p->recalcHeight();

    return p;
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
            res = distance(set, request.first, request.second);
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