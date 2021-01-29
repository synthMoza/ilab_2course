#pragma once

#include <iostream>
#include <set>
#include <chrono>
#include <cassert>
#include <vector>

#define AVLSET

namespace mfns {

struct Node {
    int key_;
    int height_;
    Node* leftChild;
    Node* rightChild;

    Node(int key) : key_ (key), height_ (1), 
        leftChild(nullptr), rightChild(nullptr) {}

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
};

// Simple right rotation of the given node
Node* rightRotate(Node* p) {
    assert(p);
    assert(p->leftChild);
    
    Node* q = p->leftChild;    
    p->leftChild = q->rightChild;
    q->rightChild = p;

    p->recalcHeight();  
    q->recalcHeight();

    return q;
}

// Simple left rotation of the given node
Node *leftRotate(Node *q)
{
    assert(q);
    assert(q->rightChild);

    Node *p = q->rightChild;
    q->rightChild = p->leftChild;
    p->leftChild = q;

    q->recalcHeight();
    p->recalcHeight();

    return p;
}

// Balances the given tree (AVL)
Node* balanceTree(Node* p) {
    assert(p);
    
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

// Finds the lowest common ancestor
Node* lowestCA(Node* head, Node* p, Node* q) {
    assert(head);
    assert(p);
    assert(q);

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

// Distance between the head and the node (not work if the nodes are from different trees)
int distanceFromHead(Node* head, Node* p) {
    assert(head);
    assert(p);

    if (head->key_ > p->key_) {
        if (head->leftChild == nullptr)
            return -1;
        return (distanceFromHead(head->leftChild, p) + 1);
    }
    else if (head->key_ < p->key_) {
        if (head->rightChild == nullptr)
            return -1;
        return (distanceFromHead(head->rightChild, p) + 1);
    }
    else
        return 0;
}

// Range-query (all values in this range), [p;q]
int distance(Node *head, int p, int q)
{
    assert(head);
    assert(p);
    assert(q);

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