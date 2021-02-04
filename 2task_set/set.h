#pragma once

#include <iostream>
#include <set>
#include <chrono>
#include <cassert>
#include <vector>
#include <stack>

std::vector<int> stdset(std::vector<int> data, std::vector<std::pair<int, int>> requests);
std::vector<int> avlset(std::vector<int> data, std::vector<std::pair<int, int>> requests);

namespace mfns {

struct Node final {
    int key_;
    int height_;
    Node* leftChild;
    Node* rightChild;

    Node(int key) : key_ (key), height_ (1), 
        leftChild(nullptr), rightChild(nullptr) {}

    Node(const Node& that) {
        key_ = that.key_;
        height_ = that.height_;
        if (that.leftChild != nullptr)
            leftChild = new Node(*(that.leftChild));
        else
            leftChild = nullptr;
        if (that.rightChild != nullptr)
            rightChild = new Node(*(that.rightChild));
        else
            rightChild = nullptr;
    }

    Node& operator=(const Node& that) {
        if (this == &that)
            return *this;
        key_ = that.key_;
        height_ = that.height_;
        if (that.leftChild)
            leftChild = new Node(*(that.leftChild));
        else
            leftChild = nullptr;
        if (that.rightChild)
            rightChild = new Node(*(that.rightChild));
        else
            rightChild = nullptr;
    }
    void print() {
        if (leftChild)
            leftChild->print();
        std::cout << key_ << " ";
        if (rightChild)
            rightChild->print();
    }

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

    ~Node() {
        if (leftChild)
            delete leftChild;
        if (rightChild)
            delete rightChild;
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

Node* balanceTree(Node* p);
Node* lowestCA(Node* head, Node* p, Node* q);
Node* insert(Node* p, int key);
int distance(Node *head, int p, int q);

}