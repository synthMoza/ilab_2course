#include "INode.hpp"

#include <queue>

using namespace se;

// INode methods

INode::INode(BaseNode* node, Symtab* table) : node_ (node), table_ (table) {}

void INode::commit() {
    int result = 0;

    if (node_ == nullptr)
        throw std::runtime_error("Unexpected null node during commit!");

    result = node_->processNode(table_);
}

INode::~INode() {
    // Scope will be deleted in other place
    // Delete expression tree
    BaseNode* tmp = nullptr;
    std::queue<BaseNode*> queue;
    queue.push(node_);

    while (!queue.empty()) {
        tmp = queue.front();
        queue.pop();

        for (auto child : tmp->children_) {
            if (child != nullptr)
                queue.push(child);
        }

        delete tmp;
    }
}

// IfNode methods

IfNode::IfNode(BaseNode* cond, Symtab* table, INode* code) : code_ (code), INode(cond, table) {}

void IfNode::commit() {
    int cond_value = 0;

    if (node_ == nullptr || code_ == nullptr)
        throw std::runtime_error("Unexpected null node during commit!");

    cond_value = node_->processNode(table_);
    if (cond_value) {
        // The condition is true
        code_->commit();
    }
}