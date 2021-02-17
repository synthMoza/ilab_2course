#pragma once

#include <iostream>
#include <set>
#include <chrono>
#include <cassert>
#include <vector>
#include <stack>

using ReqIt = std::vector<std::pair<int, int>>::iterator;
using DataIt = std::vector<int>::iterator;

std::vector<int> avlset(DataIt data_begin, DataIt data_end, ReqIt req_begin, ReqIt req_end);
std::vector<int> stdset(DataIt data_begin, DataIt data_end, ReqIt req_begin, ReqIt req_end);

// Node realisation for the set based on the AVL tree, protected from user
namespace {
template <typename KeyT>
struct Node final {
    KeyT key_;
    int height_;
    Node* leftChild;
    Node* rightChild;

    Node(KeyT key) : key_ (key), height_ (1), 
        leftChild(nullptr), rightChild(nullptr) {}
  
    // Prints all elements from the given tree (LNR)
    void print() const {
        if (leftChild)
            leftChild->print();
        std::cout << key_ << " ";
        if (rightChild)
            rightChild->print();
    }

    // Find the minimum element that is not less than the given
    Node* lower_bound(const KeyT& key) const {
        if (key_ > key) {
            if (leftChild == nullptr)
                return this;
            return leftChild->lower_bound(key);
        } else if (key_ == key) {
            // Found the element
            return const_cast<Node*>(this);
        } else {
            // key_ < key
            if (rightChild == nullptr)
                return nullptr;
            return rightChild->lower_bound(key);
        }
    }

    // Find the minimum element that is greater than the given
    Node* upper_bound(const KeyT& key) const {
        Node<KeyT>* tmp = nullptr;

        if (key_ > key) {
            if (leftChild == nullptr)
                return this;
            tmp = leftChild->upper_bound(key);
            if (tmp == nullptr)
                return const_cast<Node*>(this);
            else
                return tmp;
        } else {
            // key_ <= key
            if (rightChild == nullptr)
                return nullptr;
            return rightChild->upper_bound(key);
        }
    }
    
    // Finds the value in the tree, returns nullptr if there is no such element
    Node* find(const KeyT& key) const {
        if (key_ > key) {
            if (leftChild == nullptr)
                return nullptr;
            return leftChild->find(key);
        } else if (key_ == key) {
            // Found the element
            return const_cast<Node*>(this);
        } else {
            // key_ < key
            if (rightChild == nullptr)
                return nullptr;
            return rightChild->find(key);
        }
    }

    // Finds the lowest common ancestor
    Node* lowestCA(const Node* p, const Node* q) const {
        if (p == nullptr || q == nullptr)
            throw std::runtime_error("Unexpected nullptr in " + std::string(__PRETTY_FUNCTION__));

        int n1 = p->key_;
        int n2 = q->key_;
        int n = key_;

        if (n1 > n && n2 > n) {
            return rightChild->lowestCA(p, q);
        } else if (n1 < n && n2 < n) {
            return leftChild->lowestCA(p, q);
        } else 
            return const_cast<Node*>(this);
    }

    // Range-query (all values in this range), [p;q]
    int distance(const KeyT& p, const KeyT& q) const {
        int res = 0;

        if (key_ >= p && key_ <= q) {
            // This key is in the interval
            res++;
            if (leftChild)
                res += leftChild->distance(p, q);
            if (rightChild)
                res += rightChild->distance(p, q);
        } else if (key_ < p) {
            if (rightChild)
                res += rightChild->distance(p, q);
        } else {
            if (leftChild)
                res += leftChild->distance(p, q);
        }

        return res;
    }

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

    // Balances the given tree (AVL)
    Node* balanceTree() {
        recalcHeight();
        
        if (bFactor() == 2) {
            if (rightChild->bFactor() < 0)
                rightChild = rightChild->rightRotate();
            return leftRotate();
        }    

        if (bFactor() == -2) {
            if (leftChild->bFactor() > 0)
                leftChild = leftChild->leftRotate();
            return rightRotate();
        }

        return this;
    }

    // Simple left rotation of the given node
    Node* leftRotate() {
        if (this->rightChild == nullptr)
            throw std::runtime_error("Unexpected nullptr in " + std::string(__PRETTY_FUNCTION__));
        
        Node *p = rightChild;
        rightChild = p->leftChild;
        p->leftChild = this;

        recalcHeight();
        p->recalcHeight();

        return p;
    }
    
    // Simple right rotation of the given node
    Node* rightRotate() {
        if (leftChild == nullptr)
            throw std::runtime_error("Unexpected nullptr in " + std::string(__PRETTY_FUNCTION__));

        Node* q = leftChild;    
        leftChild = q->rightChild;
        q->rightChild = this;

        recalcHeight();  
        q->recalcHeight();

        return q;
    }

    // Finds minimum element in the given nove
    Node* findMin() const {
        return (leftChild == nullptr) ? const_cast<Node*>(this) : leftChild->findMin();
    }
};

// Inserts a new value in the given tree
// Can't be inside "class Node", as the tree may change (and "this" may be changed)
template <typename KeyT>
Node<KeyT>* insert(Node<KeyT>* p, const KeyT& key) {
    if (p == nullptr)
        return new Node<KeyT>(key);

    if (key < p->key_)
        p->leftChild = insert(p->leftChild, key);
    else
        p->rightChild = insert(p->rightChild, key);

    return p->balanceTree();
}

// Removes the minimum element from the given tree
// Can't be inside "class Node", as the tree may change ("this" may be deleted)
template <typename KeyT>
Node<KeyT>* removeMin(Node<KeyT>* p) {
    if (p->leftChild == nullptr)
        return p->rightChild;
    
    p->leftChild = removeMin(p->leftChild);
    return p->balanceTree();
}

template <typename KeyT>
Node<KeyT>* remove(Node<KeyT>* p, const KeyT& key) {
    if (p == nullptr)
        return nullptr;
    
    if (key < p->key_)
        p->leftChild = remove(p->leftChild, key);
    else if (key > p->key_)
        p->rightChild = remove(p->rightChild, key);
    else {
        Node<KeyT>* q = p->leftChild;
        Node<KeyT>* r = p->rightChild;
        delete p;
        if (r == nullptr)
            return q;
        Node<KeyT>* min = r->findMin();
        min->rightChild = removeMin(r);
        min->leftChild = q;
        return min->balanceTree();
    }

    return p->balanceTree();
}
}

namespace mfns {
template <typename KeyT>
// Class for using AVL tree for the set
class avlset final {
    Node<KeyT>* root_;
    size_t size_;

    // Removes the whole tree iteratively (using std::stack)
    void removeSet() {
        std::stack<Node<KeyT>*> stack;
        Node<KeyT>* current = root_;
        Node<KeyT>* temp = nullptr;

        while (current != nullptr || !stack.empty()) {
            while (current != nullptr) {
                stack.push(current);
                current = current->leftChild;
            }

            current = stack.top();
            stack.pop();

            // delete this node
            temp = current->rightChild;
            delete current;
            current = temp;
        }
    }
public:
    // Create empty set
    avlset() : root_ (nullptr), size_ (0) {}
    // Create set with the given element, safe about exceptions
    avlset(std::initializer_list<KeyT> list) : root_ (nullptr), size_ (list.size())  {
        avlset tmp;

        tmp.size_ = list.size();
        for (KeyT key : list) {
            tmp.insert(key);
        }
        // Kalb line

        swap(tmp);
    }
    // Copy constructor, safe about exceptions
    avlset(const avlset<KeyT>& that) {
        avlset tmp{};
        std::stack<Node<KeyT>*> stack;
        Node<KeyT>* current = that.root_;

        while (current != nullptr || !stack.empty()) {
            while (current != nullptr) {
                stack.push(current);
                current = current->leftChild;
            }

            current = stack.top();
            stack.pop();

            tmp.insert(current->key_);
            current = current->rightChild;
        }
        tmp.size_ = that.size_;
        // Kalb line
        
        size_ = 0;
        root_ = nullptr;
        swap(tmp);
    }

    avlset<KeyT>& operator=(avlset<KeyT>& that) {
        avlset<KeyT> tmp(that);
        // Kalb lane

        swap(tmp);
        return *this;
    }
    // Swaps content of two sets
    void swap(avlset<KeyT>& that) noexcept {
        std::swap(root_, that.root_);
        std::swap(size_, that.size_);
    }
    // Insert the given key into the tree
    void insert(const KeyT& key) {
        if (root_ != nullptr) {
            // Check if the element is already in the set
            if (count(key) == 0)
                root_ = ::insert(root_, key);
        }
        else
            root_ = new Node<KeyT>{key};
        
        size_++;
    }
    // Prints the elements in the set in the ascending order
    void print() const {
        if (root_ != nullptr) {
            root_->print();
            std::cout << std::endl;
        }
    }
    // Finds the element of the set
    Node<KeyT>* find(const KeyT& key) const {
        return root_->find(key);
    }
    // Returns true if the container is empty, else it returns false
    bool empty() const noexcept {
        return (root_ == nullptr) ? true : false;
    }
    // Returns current size of the set
    size_t size() const noexcept {
        return size_;
    }
    // Erases the element with the given key from the tree
    void erase(KeyT key) {
        if (root_ != nullptr)
            root_ = remove(root_, key);
    }
    // Removes all elements from the set
    void clear() {
        removeSet();
        size_ = 0;
    }
    // Counts elements with the given value
    // Returns 1 if the element is in the set, otherwise returns 0 (all elements are unique)
    size_t count(const KeyT& key) const {
        if (root_ != nullptr)
            if (root_->find(key) != nullptr)
                return 1;
        
        return 0;
    }
    // Finds the least element that is not less than the given element
    Node<KeyT>* lower_bound(const KeyT& key) const {
        if (root_ != nullptr)
            return root_->lower_bound(key);
        else
            return nullptr;
    }
     // Finds the least element that is greater than the given element
    Node<KeyT>* upper_bound(const KeyT& key) const {
        if (root_ != nullptr)
            return root_->upper_bound(key);
        else
            return nullptr;
    }
    // Returns number of values that are in the given interval [first; second]
    // Type "KeyT" must be comparable
    int distance(const KeyT& first, const KeyT& second) {
        if (root_ != nullptr)
            return root_->distance(first, second);
        else
            throw std::runtime_error("Exception in " + std::string(__PRETTY_FUNCTION__) + ": no elements in the set!" );
    }
    ~avlset() {
        removeSet();
    }
};
}