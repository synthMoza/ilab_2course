#pragma once

#include <iostream>
#include <set>
#include <chrono>
#include <cassert>
#include <vector>
#include <stack>

namespace mfns {

struct Node {
    int key_;
    int height_;
    Node* leftChild;
    Node* rightChild;

    Node(int key) : key_ (key), height_ (1), 
        leftChild(nullptr), rightChild(nullptr) {}

    // Find the minimum element that is not less that the given
    Node* lower_bound(int key) {
        if (key_ > key) {
            if (leftChild == nullptr)
                return this;
            return leftChild->lower_bound(key);
        } else if (key_ == key) {
            // Found the element
            return this;
        } else {
            // key_ < key
            if (rightChild == nullptr)
                return this;
            return rightChild->lower_bound(key);
        }
    }

    const Node* lower_bound(int key) const {
        if (key_ > key) {
            if (leftChild == nullptr)
                return this;
            return leftChild->lower_bound(key);
        } else if (key_ == key) {
            // Found the element
            return this;
        } else {
            // key_ < key
            if (rightChild == nullptr)
                return this;
            return rightChild->lower_bound(key);
        }
    }
    
    // Finds the value in the tree, returns nullptr if there is no such element
    Node* find(int key) {
        if (key_ > key) {
            if (leftChild == nullptr)
                return nullptr;
            return leftChild->find(key);
        } else if (key_ == key) {
            // Found the element
            return this;
        } else {
            // key_ < key
            if (rightChild == nullptr)
                return nullptr;
            return rightChild->find(key);
        }
    }

    const Node* find(int key) const {
        if (key_ > key) {
            if (leftChild == nullptr)
                return nullptr;
            return leftChild->find(key);
        } else if (key_ == key) {
            // Found the element
            return this;
        } else {
            // key_ < key
            if (rightChild == nullptr)
                return nullptr;
            return rightChild->find(key);
        }
    }

    void destroy() {
        std::stack<Node*> stack;
        Node* current = this;
        Node* temp = nullptr;

        while (current != nullptr || !stack.empty()) {
            while (current != nullptr) {
                stack.push(current);
                current = current->leftChild;
            }

            current = stack.top();
            stack.pop();
            temp = current->rightChild;

            delete current;

            current = temp;
        }
    }

private:
    // Calculates the "balance factor" for this tree
    int bFactor() const {
        int lheight = 0;
        int rheight = 0;

        if (leftChild)
            lheight = leftChild->height_;
        if (rightChild)
            rheight = rightChild->height_;

        return rheight - lheight;
    }

    // Recalculates the height of this tree
    void recalcHeight() {
        int height1 = (leftChild != nullptr) ? leftChild->height_ : 0;
        int height2 = (rightChild != nullptr) ? rightChild->height_ : 0;

        height_ = (height1 > height2) ? height1 : height2 + 1;
    }

    friend Node* rightRotate(Node* p);
    friend Node* leftRotate(Node *q);
    friend Node* balanceTree(Node* p);
};

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
int distance(Node *head, int p, int q)
{
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
    catch (std::exception exception) {
        std::cout << exception.what();
        set->destroy();
        exit(EXIT_FAILURE);
    }

    set->destroy();
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